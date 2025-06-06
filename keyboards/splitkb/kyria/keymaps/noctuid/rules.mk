OLED_ENABLE = no
OLED_DRIVER = SSD1306      # Enables the use of OLED displays
ENCODER_ENABLE = no        # Enables the use of one or more encoders

RGB_MATRIX_ENABLE = no     # Disable keyboard RGB matrix, as it is enabled by default on rev3
# can't compile with this
RGBLIGHT_ENABLE = no       # Enable keyboard RGB underglow
LTO_ENABLE = no

# extra features
CAPS_WORD_ENABLE = yes
OS_DETECTION_ENABLE = yes
LAYER_LOCK_ENABLE = yes
# support qmk console command
CONSOLE_ENABLE = yes
TAP_DANCE_ENABLE = yes
KEY_OVERRIDE_ENABLE = yes

# using liatris controller
CONVERT_TO=liatris
