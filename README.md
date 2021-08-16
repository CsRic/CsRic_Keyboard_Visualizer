# CsRic_Keyboard_Visualizer
A Razer Chroma plugin developed with WASAPI and ChromaSDK. Capture audio streams and visualize them on Razer keyboard.   
This application runs on windows 10.   
## Instruction  
The project has two parts.   
The CoreFunction part, which captures any audio streams being played and analyzes frequency by CQT and visualizes on keyboard, will be compiled as an dll. I use C++ in this part.   
The other part is a front-end windows dekstop application, which calls the CoreFunction dll and show frequncy histograms on the form. I use C-sharp in this part.  


