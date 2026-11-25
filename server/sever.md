python --version
Python 3.12.6

python -m venv .venv
.venv\Scripts\activate
pip install fastapi uvicorn

python.exe -m pip install --upgrade pip


uvicorn main:app --host 0.0.0.0 --port 8000


check connectivity

Invoke-RestMethod -Uri http://localhost:8000/api/v1/readings -Method Post `
  -Headers @{ "X-Node-Key" = "change-me-school-key" } `
  -ContentType "application/json" `
  -Body '{"v":1,"node":"test","ts":1732450800,"sensors":{"temp_c":21.4}}'

  check output
  type data\test\2024-11-24.csv