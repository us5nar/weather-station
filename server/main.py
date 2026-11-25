# server/main.py
import csv, time, threading
from pathlib import Path
from fastapi import FastAPI, Header, HTTPException
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel

DATA_DIR = Path("data")
API_KEYS = {"change-me-school-key"}   # one shared key for the demo
app = FastAPI(title="Weather Station")
_lock = threading.Lock()              # serializes CSV writes

class Reading(BaseModel):
    v: int
    node: str
    ts: int
    sensors: dict[str, float]

@app.post("/api/v1/readings", status_code=201)
def post_reading(r: Reading, x_node_key: str = Header(...)):
    if x_node_key not in API_KEYS:
        raise HTTPException(401, "bad key")
    day = time.strftime("%Y-%m-%d", time.gmtime(r.ts))
    path = DATA_DIR / r.node / f"{day}.csv"
    path.parent.mkdir(parents=True, exist_ok=True)
    is_new = not path.exists()
    received = int(time.time())       # server clock, for drift comparison
    with _lock, path.open("a", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        if is_new:
            w.writerow(["node", "ts", "server_ts", "sensor", "value"])
        for key, val in r.sensors.items():
            w.writerow([r.node, r.ts, received, key, val])
    return {"ok": True}

@app.get("/api/v1/nodes")
def list_nodes():
    if not DATA_DIR.exists():
        return []
    return sorted(p.name for p in DATA_DIR.iterdir() if p.is_dir())

# Mount last so the API routes take priority over static files
app.mount("/", StaticFiles(directory="static", html=True), name="static")