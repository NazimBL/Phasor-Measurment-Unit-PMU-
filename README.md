# PMU


On This project we intended to implement a prototype for a low-cost based M
(Measurement) Class Phasor Measurement Unit (PMU) oriented for research and
education use cases. The Analog Device ADE7880 is used with a 32-bit MCU to
calculate the phasors in addition to frequency and power of a three phase system.
The Pulse per Second (PPS) signal from a Sim808 Global Positioning System (GPS)
module is used to generate the sampling pulses and synchronize the sampling process
done by the Analog to Digital Converters (ADC) of the ADE7880. The reference
time is also obtained from the GPS with which the phasors are time stamped and
sent via Global Packet Radio Service (GPRS) and displayed in our Synchro Phasor
Visualization Dashboard. The data collected from PMUs is stored and processed
for visualization and future use.
