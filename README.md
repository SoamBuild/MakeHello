# MakeHello

**Make hello** o **Timbre interactivo** es un proyecto realizado por
- Marcos Arce
- Javier Clavero
- Rebeca Pizarro

Con el objetivo que el timbre fuese realmente interactivo, evitamos utilizar botones y sensores de proximidad, ya que nos resultaba poco interesante realizar otro timbre que se active con acciones que ya conocemos. 

Este  proyecto utiliza gestos para activar un timbre y detecta el gesto de saludar 👋 y posteriormente envia una señal a otros microcontroladores para avisar que alguien se encuentra en la puerta esperando.

Para detectar esta accion 👋, utilizamos un sensor de gestos PAJ7620 que mediante comunicacion I2C, permite  detectar hasta 9 gestos, dentro de esos el izquierda y derecha los necesarios para detectar la accion de 👋.

## Materiales 

Este proyecto se divide en dos partes, la primera un microcontrolador que envia una señal y otro que recibe esa señal.

### SENDER 

- Wemos d1 mini esp8266 X1
- PAJ7620 X1
- Tira de neopixeles X8

### RECEPTOR

- Wemos d1 mini esp8266 X1
- Mini motor vibrador PWM
- Tira de neopixeles X3


## Funcionamiento 

La comunicacion entre las dos partes del proyecto se realiza inalambricamente mediante el uso de ESPNOW(), y en este repositorio encontraras dos codigos.

### Code_Receptor

Code receptor se compone de distintas funciones que se ejecutan tanto en el Setup y Loop.
 
**-onboot_Mode** que ejecuta un rutina de encendido y apagado en funcion de la cantidad de neopixeles que se establezca en el codigo previo al void setup.

**standby_Mode** Este modo o rutina se ejecuta permanentemente y realiza un breve cambio en el brillo de los neopixeles y este cambia cuando llega un nuevo mensaje desde el microcontrolador "Maestro"

**alert_Mode** El modo alerta realiza un rapido parpadeo en color rojo de todos los leds que tengas declarado para la tira y se detiene cuando el microcontrolador "Maestro" cambia el estado de la variable.

### Sensor_Sender

Sensor_sender ademas de detectar y registrar los gestos que el sensor detecte es el encargado de enviar los distintos mensajes a los microcontroladores receptores y tiene distintos modos que se ejecutan durante el setup y loop

**-onboot_Mode** ejecuta una rutina de encendido y apagado en funcion de la cantidad de neopixeles que se establezca en el codigo previo al void setup.

**standby_Mode** Este modo es el encargado de comunicar el gesto realizado mediante un barrido de los leds encendiendo en un color cyan, es decir si paso mi mano haciendo el gesto hacia la izquierda los neopixeles realizan una rutina que replica ese movimiento mediante luz.

**alert_Mode** El modo alerta realiza un rapido parpadeo en color rojo de todos los leds que tengas declarado para la tira y se detiene cuando el microcontrolador "Maestro" cambia el estado de la variable.

## Futuras actualizaciones

- Finalizar la integracion de los motores vibradores en el microcontrolador receptor
- Agregar nuevos modos a partir de los Neopixeles
- Desarrollar carcasas para ambos dispositivos 
- Desarrollar pcbs para ambos dispositivos  
