"""
Credits go to: https://github.com/Zulko/pianoputer
"""

from scipy.io import wavfile
import numpy as np
import pygame


def speedx(snd_array, factor):
    """ Speeds up / slows down a sound, by some factor. """
    indices = np.round(np.arange(0, len(snd_array), factor))
    indices = indices[indices < len(snd_array)].astype(int)
    return snd_array[indices]


def stretch(snd_array, factor, window_size, h):
    """ Stretches/shortens a sound, by some factor. """
    phase = np.zeros(window_size)
    hanning_window = np.hanning(window_size)
    result = np.zeros(int(len(snd_array) / factor + window_size))

    for i in np.arange(0, len(snd_array) - (window_size + h), h*factor):
        i = int(i)
        # Two potentially overlapping subarrays
        a1 = snd_array[i: i + window_size]
        a2 = snd_array[i + h: i + window_size + h]

        # The spectra of these arrays
        s1 = np.fft.fft(hanning_window * a1)
        s2 = np.fft.fft(hanning_window * a2)

        # Rephase all frequencies
        phase = (phase + np.angle(s2/s1)) % 2*np.pi

        a2_rephased = np.fft.ifft(np.abs(s2)*np.exp(1j*phase))
        i2 = int(i/factor)
        result[i2: i2 + window_size] += hanning_window*a2_rephased.real

    # normalize (16bit)
    result = ((2**(16-4)) * result/result.max())

    return result.astype('int16')


def pitchshift(snd_array, n, window_size=2**13, h=2**11):
    """ Changes the pitch of a sound by ``n`` semitones. """
    factor = 2**(1.0 * n / 12.0)
    stretched = stretch(snd_array, 1.0/factor, window_size, h)
    return speedx(stretched[window_size:], factor)


def init_sounds():
    fps, sound = wavfile.read('bowl.wav')

    tones = range(-25, 25)
    print('initializing the sounds')
    transposed_sounds = [pitchshift(sound, n) for n in tones]
    print('done')

    return transposed_sounds


from flask import Flask, request

# Preprocess the keys and notes
app = Flask(__name__)
fps, _ = wavfile.read('bowl.wav')
transposed_sounds = init_sounds()
pygame.mixer.init(fps, -16, 1, 2048)
sounds = map(pygame.sndarray.make_sound, transposed_sounds)
key_sound = list(sounds)
print('There are {} keys to be played from 0 to {}'.format(
    len(key_sound), len(key_sound) - 1))


@app.route("/")
def play():
    """
    index route, playing the note.

    accepts: /?key={key}

    e.g. /?key=10 to play note number 10
    """
    key = int(request.args.get('key'))
    print('Playing key {}'.format(key))
    key_sound[key].play(fade_ms=50)
    return 'Playing key {}'.format(key)


# start the server at the port 5000
app.run(host = '0.0.0.0')
