from pyfirmata import Arduino, util
from cvzone.HandTrackingModule import HandDetector
import cv2
import time

# Inicialização do Arduino/ESP32 com PyFirmata 
# Certifique-se de substituir /dev/ttyUSB4 pela porta correcta onde o ESP está conectado
board = Arduino('/dev/ttyUSB4')
it = util.Iterator(board)
it.start()

# Pino do LED no ESP32 
led_pin = 2

# Configuração do detector de mão
cap = cv2.VideoCapture(0)  # Captura da câmera obs: verifique o index da sua câmera, pode ser 0, 1 ou 3
detector = HandDetector(staticMode=False, maxHands=2, modelComplexity=1, detectionCon=0.5, minTrackCon=0.5)

# Funções para controle do LED
def set_led_state(state):
    board.send_sysex(0x01, [state])

def turn_on_led():
    set_led_state(0x01)  # Acender o LED
    print("LED aceso")

def turn_off_led():
    set_led_state(0x00)  # Apagar o LED
    print("LED apagado")

# Loop principal
while True:
    success, img = cap.read()  # Captura o próximo quadro da câmera
    hands, img = detector.findHands(img)  # Detecta as mãos na imagem

    if hands:
        hand1 = hands[0]  # Pega a primeira mão detectada
        fingers_up = detector.fingersUp(hand1)
        print(f"Dedos levantados: {fingers_up}")  # Mensagem de depuração

        # Se todos os dedos estão levantados, acende o LED; caso contrário, apaga
        if fingers_up.count(1) == 5:  # Todos os dedos levantados
            turn_on_led()
        else:
            turn_off_led()

    cv2.imshow("Image", img)  # Mostra a imagem com as detecções
    cv2.waitKey(1)  # Espera pela tecla para fechar

board.exit() 
