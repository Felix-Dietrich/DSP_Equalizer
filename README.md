# DSP_Equalizer


## compile code 
### on mac
clang -o DSP DSP.c


## Ziele
Filtervorgabe als min/max Dämpfung in bestimmtem Frequenzbereich.

Zeitmessung auf Mac m3 und Mikrocontroller
Anforderung 44100Hz audio in Echtzeit verarbeiten auf Raspberry Pi pico.


die Frequenzkanäle werden folgendermassen gewählt: 

25*(2^(n/2))
n= 0 ... 19



Filter als C-Code implementiert
- FIR-Filter mit Faltung 
- FIR-Filter mit FFT
- IIR-Filter biquad Filter
- DWT-Filter als Filterbank

## Main
- liest und schreibt raw audio file
- liest spektrum konfiguration

## Filterfunktion
- parameter: audioblock
- parameter: blockgrösse
- parameter: spektrum



## Visualizer in python
Bedienung:
* audiodatei wählen
* spektrum vorgeben
* filtertyp wählen


Auswertung:
* Prozesszeit intel
* Prozesszeit M0
* THD sinus 1K
* genaues Spektrum FFT von Impulsantwort mit zero padding

## Weitere:
- welchen Einfluss auf die Qualität hat die Bittiefe
- Kann mit floating point eine bessere Performance erreicht werden?