#!/usr/bin/env python3

import shutil
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parent.parent
PNG_DIR = ROOT / "renders" / "png"
shutil.rmtree(PNG_DIR, ignore_errors=True)
PNG_DIR.mkdir(parents=True, exist_ok=True)

for file in (ROOT / "renders" / "ppm").glob("*.ppm"):
    out = PNG_DIR / f"{file.stem}.png"
    Image.open(file).save(out)
    print(f"wrote {out}")

