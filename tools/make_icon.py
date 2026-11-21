"""Package the app artwork into the sizes used by Windows Explorer and the tray."""
from pathlib import Path

from PIL import Image

root = Path(__file__).resolve().parents[1]
sizes = [16, 20, 24, 32, 40, 48, 64, 128, 256]
with Image.open(root / 'docs/images/ultimate-desktop-effects.png') as image:
    icon = image.convert('RGBA')
    icon.save(root / 'res/ultimate.ico', sizes=[(size, size) for size in sizes])
print('Built Ultimate Desktop Effects ICO frames:', sizes)
