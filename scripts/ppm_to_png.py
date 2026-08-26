#!/usr/bin/env python3

from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parent.parent
PNG_DIR = ROOT / "renders" / "png"
PNG_DIR.mkdir(parents=True, exist_ok=True)

for file in (ROOT / "renders" / "ppm").glob("*.ppm"):
    Image.open(file).save(PNG_DIR / (file.stem + ".png"))
    print(f"wrote {file.stem}.png")

