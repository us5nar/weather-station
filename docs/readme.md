The wether and air quality control station based on ESP8266

 ┌──────────────────────┐        HTTP POST JSON         ┌────────────────────────┐
 │ Sensor node (ESP8266)│ ───────────────────────────▶  │ Central server         │
 │ - SDS011 (UART)      │   every N minutes             │ - REST API (FastAPI)   │
 │ - BME280 (I2C)       │                               │ - Storage: CSV → SQLite│
 │ - CO2 sensor (UART/I2C)                              │   → MySQL (later)      │
 │ - Config web UI      │ ◀─── (optional) GET config ── │ - Serves web dashboard │
 └──────────────────────┘                               └───────────┬────────────┘
                                                                    │ HTTP/JSON
                                                        ┌───────────▼────────────┐
                                                        │ Browser dashboard      │
                                                        │ (HTML + Chart.js)      │
                                                        └────────────────────────┘