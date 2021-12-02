import numpy as np
import matplotlib.pyplot as plt
import scipy.signal

def compute_and_plot_fft(signal, Fs):
  """Calcula e Plota a FFT de um sinal

  Args:
      signal (np.array()): Sinal de entrada
      Fs (int): Freq. de amostragem
  """
  N = len(signal) # qtd. de amostras
  
  t_step = 1/Fs   # Passo de tempo
  t = np.linspace(0, (N-1)*t_step, N) # Base de tempo

  f_step = Fs / N  # Passo de freq.
  freq = np.linspace(0, (N-1)*f_step, N)  # Base de freq

  X_fft = np.fft.fft(signal)  # Transformada do sinal
  X_mag = np.abs(X_fft)/N     # Magnitude da transformada

  freq_plot = freq[0:int(N/2 + 1)]        # Freq. para plotagem
  X_mag_plot = 2 * X_mag[0:int(N/2 + 1)]  # Magnitude da transformada para plotagem
  X_mag_plot[0] = X_mag_plot[0]/2         # A componente DC não deve ser multiplicada

  plt.rcParams.update({'font.size': 20})

  plt.figure(1, figsize=(20,10))
  plt.plot(t, signal)
  plt.title("Time Domain Signal", fontweight="bold")
  plt.xlabel("Time (s)")
  plt.ylabel("Amplitude")
  plt.xlim(0, t[-1])
  plt.grid()

  plt.figure(2, figsize=(20,10))
  plt.plot(freq_plot, X_mag_plot)
  plt.title("Frequency Domain Signal", fontweight="bold")
  plt.xlabel("Frequency (Hz)")
  plt.ylabel("Amplitude")
  plt.xlim(0, freq_plot[-1])
  plt.grid()
  
  plt.tight_layout()
  plt.show()

  return X_fft


def transform_FFT(img):
  
  f = np.fft.fft2(img)
  f = np.fft.fftshift(f)
  magnitude_spectrum = 20*np.log(np.abs(f))
  magnitude_spectrum = np.asarray(magnitude_spectrum, dtype=np.uint8)
  return magnitude_spectrum


def FFT(f):
    N = len(f)
    if N <= 1:
        return f
    
    # division
    even= FFT(f[0::2])
    odd = FFT(f[1::2])

    # store combination of results
    temp = np.zeros(N).astype(np.complex64)
    
    # only required to compute for half the frequencies 
    # since u+N/2 can be obtained from the symmetry property
    for u in range(N//2):
        temp[u] = even[u] + np.exp(-2j*np.pi*u/N) * odd[u] # conquer
        temp[u+N//2] = even[u] - np.exp(-2j*np.pi*u/N)*odd[u]  # conquer
                
    return temp


def DFT1D(f):
    # create empty array of complex coefficients
    F = np.zeros(f.shape, dtype=np.complex64)
    n = f.shape[0]

    # creating indices for x, allowing to compute the multiplication using numpy (f*exp)
    x = np.arange(n)
    # for each frequency 'u', perform vectorial multiplication and sum
    for u in np.arange(n):
        F[u] = np.sum(f*np.exp( (-1j * 2 * np.pi * u*x) / n ))
    
    return F


def myFFT(f):
  # O sinal deve ser potência de 2
    N = len(f)
    if N <= 1:
        return f
    
    even_x = myFFT(f[0::2])
    odd_x  = myFFT(f[1::2])

    F = np.zeros(N, dtype='complex')
    
    for n in range(N//2):
        W = np.exp(-2j*np.pi*n/N)
        F[n] = even_x[n] + W * odd_x[n]
        F[n+N//2] = even_x[n] - W*odd_x[n]

    return F


def correlate_and_plot(sig1, sig2, plot=True):
  
  # Cálculo da correlação adimensionalizada
  sig1_a = sig1/(np.sqrt(np.sum(sig1**2)))
  sig2_a = sig2/(np.sqrt(np.sum(sig2**2)))
  corrad = scipy.signal.correlate(sig1_a, sig2_a)

  length = len(sig1_a) + len(sig2_a) - 1
  lag = np.arange(0, length)

  if plot:
    plt.figure(figsize=(10,6))
    plt.title("")
    plt.xlabel("Atraso")
    plt.ylabel("Rxaya")
    plt.plot(lag, corrad)
    
    print("Maximo = ", corrad.max())
    print("Minimo = ", corrad.min())

  return corrad.max()  # Retorna o máximo valor da correlação


def remove_noise(signal, max_amplitude=50):
  N = len(signal)                         # qtd. de amostras

  X_fft = myFFT(signal)                   # Transformada do sinal
  X_mag = 2 * np.abs(X_fft)/N
  X_mag[0] = X_mag[0]/2
  indices = X_mag > max_amplitude

  X_fft = X_fft * indices

  noiseless_signal = np.fft.ifft(X_fft)
  
  return noiseless_signal



class FIRFilter():

  def __init__(self, fc, transition_width, stopband_attenuation=20, passband_ripple=0.1, Fs=20e3, window=None):
    """Class for designing a FIR filter

    Args:
        fc (int): Cutoff frequency, in Hertz
        transition_width (int): Transition Width, in Hertz
        stopband_attenuation (int, optional): Stopband Attenuation, in decibels. Defaults to 20.
        passband_ripple (float, optional): Passband Ripple, in decibels. Defaults to 0.1.
        Fs (int): Sampling Frequency, in Hertz. Defaults to 20kHz.
        window (string, optional): Window function used for filter design. Defaults to None.
    """
    self._fc = fc
    self._df = transition_width
    self._As = stopband_attenuation
    self._pb_ripple = passband_ripple
    self._Fs = Fs
    self._window = window

    self.__normalizeFrequencies()

    if self._window == None:
      self.__setWindow()


  def __normalizeFrequencies(self):
    self._dfn = self._df / self._Fs
    self._fcn = self._fc / self._Fs + self._dfn/3


  def __setWindow(self):
    if self._As <= 21 and self._pb_ripple >= 0.7416:
      self._window = "rectangular"
      N = np.ceil( 0.9/self._dfn )
    elif self._As <= 44 and self._pb_ripple >= 0.0546:
      self._window = "hanning"
      N = np.ceil( 3.1/self._dfn )
    elif self._As <= 53 and self._pb_ripple >= 0.0194:
      self._window = "hamming"
      N = np.ceil( 3.3/self._dfn )
    elif self._As <= 74 and self._pb_ripple >= 0.0017:
      self._window = "blackman"
      N = np.ceil( 5.5/self._dfn )
    else:
      self._window = "kaiser"
      N = np.ceil((self._As - 7.95)/(14.36*self._dfn))
    if not N%2:
      N += 1
    self._N = int(N)


  def setWindow(self, window):
    self._window = window


  def getWindow(self):
    return self._window


  def getWindowFunction(self):

    n = np.array([np.arange(-(self._N-1)/2, self._N/2)])
    
    if self._window == "rectangular":
      W = np.ones(self._N)

    elif self._window == "hanning":
      W = 0.5 + 0.5*np.cos(2*np.pi*n/self._N)

    elif self._window == "hamming":
      W = 0.54 + 0.46*np.cos(2*np.pi*n/self._N)

    elif self._window == "blackman":
      W = 0.42 + 0.5*np.cos(2*np.pi*n/(self._N-1)) + 0.08*np.cos(4*np.pi*n/(self._N-1))

    elif self._window == "kaiser":
      W = np.kaiser(self._N, 14)

    self._W = W
    self._n = n

    return self._n.T, self._W.T


  def truncateImpulse(self, hd):
    self._b = np.multiply(self._W, hd)
  

  def applyFilter(self, signal, plot=False):
    y = np.convolve(signal, self._b.reshape(self._N))
    if plot:
      plt.figure(1, figsize=(20,12))
      plt.plot(y)
      plt.xlabel("Número de amostras")
      plt.ylabel("Amplitude")
      plt.title("Sinal filtrado", fontweight="bold")
    return y


  def getFilterCoeffs(self):
    return self._b


  def getNumCoeffs(self):
    return self._N


  def plotWindow(self):
    plt.figure(1, figsize=(20,12))
    plt.stem(self._n.T, self._W.T, use_line_collection=True)
    plt.xlabel("Número de coeficientes")
    plt.ylabel("Amplitude")
    plt.title("Janela " + self._window, fontweight="bold")


  def plotCoeffs(self):
    plt.figure(1, figsize=(20,12))
    plt.stem(self._n.T, self._b.T, use_line_collection=True)
    plt.xlabel("Número de coeficientes")
    plt.ylabel("Amplitude")
    plt.title("Coeficientes do Filtro FIR (b)", fontweight="bold")


  def plotFrequencyResponde(self, db=True):
    wf, hf = freqz(self._b.T, 1, worN=1024, fs=self._Fs)
    plt.figure(1, figsize=(20,12))
    if db:
      plt.plot(wf, 20*np.log10(np.absolute(hf)))
      plt.ylabel("Ganho (dB)")
    else:
      plt.plot(wf, np.absolute(hf))
      plt.ylabel("Ganho")
    plt.xlabel("Frequencia (Hz)")
    plt.title("Resposta em Frequência", fontweight="bold")



class LowPassFIR(FIRFilter):

  def getImpulseResponse(self):
    wcn = 2*np.pi*self._fcn
    nn = np.arange(1, self._N/2)
    hd0 = np.array([[2*self._fcn]])
    hdr = 2*self._fcn*np.sin(nn*wcn)/(nn*wcn)
    hdr = np.array([hdr])
    hdl = np.flip(hdr)
    self._hd = np.concatenate((hdl, hd0, hdr), axis=1)
    return self._hd

  def getFilterCoeffs(self):
    self.getWindowFunction()
    self.getImpulseResponse()
    self.truncateImpulse(self._hd)
    return self._b

  def plotFilter(self):
    plt.figure(1, figsize=(20,12))
    plt.stem(self._n.T, self._hd.T, use_line_collection=True)
    plt.xlabel("Número de coeficientes")
    plt.ylabel("Amplitude")
    plt.title("Filtro Passa Baixas ideal", fontweight="bold")


class HighPassFIR(FIRFilter):
  
  def getImpulseResponse(self):
    wcn = 2*np.pi*self._fcn
    nn = np.arange(1, self._N/2)
    hd0 = np.array([[1 - 2*self._fcn]])
    hdr = -2*self._fcn*np.sin(nn*wcn)/(nn*wcn)
    hdr = np.array([hdr])
    hdl = np.flip(hdr)
    self._hd = np.concatenate((hdl, hd0, hdr), axis=1)
    return self._hd

  def getFilterCoeffs(self):
    self.getWindowFunction()
    self.getImpulseResponse()
    self.truncateImpulse(self._hd)
    return self._b

  def plotFilter(self):
    plt.figure(1, figsize=(20,12))
    plt.stem(self._n.T, self._hd.T, use_line_collection=True)
    plt.xlabel("Número de coeficientes")
    plt.ylabel("Amplitude")
    plt.title("Filtro Passa Altas ideal", fontweight="bold")


class BandPassFIR(FIRFilter):

  def __init__(self, fc1, fc2, transition_width, stopband_attenuation=20, passband_ripple=0.1, Fs=8e3, window=None):
    """Class for designing a BandPass FIR filter

    Args:
        fc1 (int): Lowest cutoff frequency, in Hertz
        fc2 (int): Highest cutoff frequency, in Hertz
        transition_width (int): Transition Width, in Hertz
        stopband_attenuation (int, optional): Stopband Attenuation, in decibels. Defaults to 20.
        passband_ripple (float, optional): Passband Ripple, in decibels. Defaults to 0.1.
        Fs (int): Sampling Frequency, in Hertz. Defaults to 20kHz.
        window (string, optional): Window function used for filter design. Defaults to None.
    """
    super().__init__(fc1, transition_width, stopband_attenuation, passband_ripple, Fs, window)
    self._fc1 = fc1
    self._fc2 = fc2
    self._fc1n = fc1/Fs - self._dfn/3
    self._fc2n = fc2/Fs + self._dfn/3

  def getImpulseResponse(self):
    wc1n = 2*np.pi*self._fc1n
    wc2n = 2*np.pi*self._fc2n
    nn = np.arange(1, self._N/2)
    hd0 = np.array([[2*(self._fc2n - self._fc1n)]])
    hdr = 2*self._fc2n*np.sin(nn*wc2n)/(nn*wc2n) - 2*self._fc1n*np.sin(nn*wc1n)/(nn*wc1n)
    hdr = np.array([hdr])
    hdl = np.flip(hdr)
    self._hd = np.concatenate((hdl, hd0, hdr), axis=1)
    return self._hd

  def getFilterCoeffs(self):
    self.getWindowFunction()
    self.getImpulseResponse()
    self.truncateImpulse(self._hd)
    return self._b

  def plotFilter(self):
    plt.figure(1, figsize=(20,12))
    plt.stem(self._n.T, self._hd.T, use_line_collection=True)
    plt.xlabel("Número de coeficientes")
    plt.ylabel("Amplitude")
    plt.title("Filtro Passa Faixa ideal", fontweight="bold")


class BandStopFIR(FIRFilter):

  def __init__(self, fc1, fc2, transition_width, stopband_attenuation=20, passband_ripple=0.1, Fs=8e3, window=None):
    """Class for designing a BandStop FIR filter

    Args:
        fc1 (int): Lowest cutoff frequency, in Hertz
        fc2 (int): Highest cutoff frequency, in Hertz
        transition_width (int): Transition Width, in Hertz
        stopband_attenuation (int, optional): Stopband Attenuation, in decibels. Defaults to 20.
        passband_ripple (float, optional): Passband Ripple, in decibels. Defaults to 0.1.
        Fs (int): Sampling Frequency, in Hertz. Defaults to 20kHz.
        window (string, optional): Window function used for filter design. Defaults to None.
    """
    super().__init__(fc1, transition_width, stopband_attenuation, passband_ripple, Fs, window)
    self._fc1 = fc1
    self._fc2 = fc2
    self._fc1n = fc1/Fs - self._dfn/3
    self._fc2n = fc2/Fs + self._dfn/3

  def getImpulseResponse(self):
    wc1n = 2*np.pi*self._fc1n
    wc2n = 2*np.pi*self._fc2n
    nn = np.arange(1, self._N/2)
    hd0 = np.array([[1 - 2*(self._fc2n - self._fc1n)]])
    hdr = 2*self._fc1n*np.sin(nn*wc1n)/(nn*wc1n) - 2*self._fc2n*np.sin(nn*wc2n)/(nn*wc2n)
    hdr = np.array([hdr])
    hdl = np.flip(hdr)
    self._hd = np.concatenate((hdl, hd0, hdr), axis=1)
    return self._hd

  def getFilterCoeffs(self):
    self.getWindowFunction()
    self.getImpulseResponse()
    self.truncateImpulse(self._hd)
    return self._b

  def plotFilter(self):
    plt.figure(1, figsize=(20,12))
    plt.stem(self._n.T, self._hd.T, use_line_collection=True)
    plt.xlabel("Número de coeficientes")
    plt.ylabel("Amplitude")
    plt.title("Filtro Rejeita Faixa ideal", fontweight="bold")
