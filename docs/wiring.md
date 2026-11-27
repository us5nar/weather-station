Wiring (NodeMCU v2)
Sensor	Pin	NodeMCU
BME280	VCC / GND	3V3 / G
BME280	SDA / SCL	D2 / D1
SDS011	5V / GND	VIN (5V) / G
SDS011	TX →	D5 (RX)
SDS011	RX ←	D6 (TX)

MQ-135	Connection
VCC	VIN (5 V)
GND	G
AO	one end of R1 (220 kΩ)

MQ-135 AO ──[ R1 220k ]──┬──► NodeMCU A0
                         │
                      [ R2 47k ]
                         │
                        GND


47k / (220k + 47k) ≈ 0.176, so 5 V >> 0.88 V at A0 (less ten 1V)

The MQ-135 heater draws roughly 150 mA
