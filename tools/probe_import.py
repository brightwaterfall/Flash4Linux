from pywinauto import Application, keyboard, Desktop
import time, os, sys

sys.stdout.reconfigure(line_buffering=True)
ROOT = r'E:\Freelancer project\2026\10\QT migration'
EXE = os.path.join(ROOT, 'bin', 'f4l.exe')
os.environ['PATH'] = r'C:\Qt\Tools\mingw730_64\bin;C:\Qt\5.12.12\mingw73_64\bin;' + os.environ.get('PATH', '')
os.system('taskkill /F /IM f4l.exe >NUL 2>&1')
time.sleep(0.5)

app = Application(backend='uia').start('"%s"' % EXE, work_dir=ROOT)
time.sleep(3.5)
win = app.window(title_re=r'^F4lm')
win.wait('visible', timeout=15)
win.set_focus()
print('main', win.window_text(), flush=True)

keyboard.send_keys('%f')
time.sleep(1.0)
item = None
for c in win.descendants():
    try:
        if c.element_info.control_type == 'MenuItem' and c.window_text() == 'Import...':
            item = c
            break
    except Exception:
        pass
print('item found', bool(item), flush=True)
if item:
    r = item.rectangle()
    print('item rect', r, flush=True)
    from pywinauto import mouse
    mouse.click(coords=(r.mid_point().x, r.mid_point().y))
    print('clicked', flush=True)
time.sleep(3.0)

print('--- windows ---', flush=True)
for w in Desktop(backend='uia').windows():
    try:
        t = w.window_text() or ''
        if t:
            print('DESK', repr(t)[:80], w.element_info.control_type, flush=True)
    except Exception:
        pass

# search all descendants under process for File name / Import
print('--- scan ---', flush=True)
for c in win.descendants():
    try:
        t = c.window_text() or ''
        if any(k in t for k in ('Import', 'File name', 'Look in', 'Open', 'Cancel')):
            print('HIT', repr(t), c.element_info.control_type, flush=True)
    except Exception:
        pass

keyboard.send_keys('{ESC}{ESC}')
time.sleep(0.3)
app.kill()
print('done', flush=True)
