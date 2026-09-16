#!/usr/bin/env python3

import shutil
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parent.parent
PPM_DIR = ROOT / "renders" / "ppm"
GIF_DIR = ROOT / "renders" / "gif"

shutil.rmtree(GIF_DIR, ignore_errors=True)
GIF_DIR.mkdir(parents=True, exist_ok=True)

for type in ("framebuffer", "zbuffer"):
    frames = sorted(PPM_DIR.glob(f"*_{type}.ppm"))
    scene = frames[0].name.split(f"_{type}.ppm")[0].rstrip("0123456789")
    images = [Image.open(f) for f in frames]
    out = GIF_DIR / f"{scene}_{type}.gif"
    images[0].save(out, save_all=True, append_images=images[1:], duration=50, loop=0)
    print(f"wrote {out}")

