import matplotlib.pyplot as plt
import numpy as np
from matplotlib.widgets import Slider

anzahl = 32
spectrum = np.ones(anzahl)
samples = np.zeros((anzahl-1)*2)
samples2= np.zeros((anzahl-1)*2)

# Create a function to update the spectrum when a slider is moved
def update(val):
    for i in range(anzahl):
        spectrum[i] = sliders[i].val
    samples=np.fft.fftshift(np.fft.irfft(spectrum))
    scatter.set_offsets(np.column_stack((np.arange((anzahl-1)*2), samples)))

    samples2= np.zeros((anzahl-1)*2)
    samples3= np.zeros((anzahl-1)*2)
    spectrum[0]=spectrum[0]*0.5
    spectrum[anzahl-1]=spectrum[anzahl-1]*0.5
    for i in range(anzahl):
        for y in range(anzahl):
            samples2[i] += 1/(anzahl)*np.cos(np.pi/(anzahl-1)*i*y)*spectrum[y];
            
    samples3 = samples2.copy()
    for i in range(anzahl-1):
        samples2[anzahl-1+i]=samples3[i]
        samples2[i]=samples3[anzahl-1-i]

    #print(max(samples2-samples))
    
    scatter2.set_offsets(np.column_stack((np.arange((anzahl-1)*2), samples2)))
    fig.canvas.draw_idle()

fig, (ax, ax2) = plt.subplots(2)
ax.set_ylim(-0.2, 1.5)
ax.set_xlim(0, (anzahl-1)*2)
scatter = ax.scatter(np.arange((anzahl-1)*2), samples, s=50)

ax2.set_ylim(-0.2, 1.5)
ax2.set_xlim(0, (anzahl-1)*2)
scatter2 = ax2.scatter(np.arange((anzahl-1)*2), samples2, s=50)

fig.subplots_adjust(top=0.66)

# Create a horizontal subplot for sliders
slider_width = 0.33/anzahl  
slider_spacing = 0.5/anzahl
sliders = []
for i in range(anzahl):
    slider_x = 0.12 + i * (slider_width + slider_spacing)  # Y-coordinate for the slider
    slider_ax = plt.axes([slider_x, 0.75, slider_width,0.2 ])  # [x, y, width, height]
    slider = Slider(slider_ax,f"", 0, 8, valinit=spectrum[i], orientation="vertical")
    slider.on_changed(update)
    sliders.append(slider)

update(0)


plt.show()
