SRC += muse.c
BACKLIGHT_ENABLE = yes
BACKLIGHT_BREATHING = yes
TAP_DANCE_ENABLE = yes
MOUSEKEY_ENABLE = yes

# Enables Link Time Optimization, saving a significant amount of space.
EXTRAFLAGS += -flto

