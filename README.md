# Previsor do Tempo

Bem-vindo ao **Previsor do Tempo**, projeto final da disciplina **Microcontroladores e Aplicações**, ministrada pelo **Departamento de Computação DC/UFSCar**.

O projeto "Previsor do Tempo" consiste num LCD colorido (RGB) e um motor. O primeiro mostra a temperatura atual numa cidade, que pode ser trocada apertando um botão, e a cor do mesmo varia com a temperatura corrente em cada uma das cidades (por exemplo, se estiver mais de 35 graus Celsius em Aracaju, o display ficará vermelho). Caso seja detectada chuva, o motor será acionado para fechar uma janela.

![Previsor do Tempo - Circuito Final.](https://github.com/alanbrandini/Previsor-do-Tempo/blob/master/Imagens/Previsor%20do%20Tempo%20-%20Completo.JPG)
![Previsor do Tempo - Circuito Final - Mostrador LCD.](https://github.com/alanbrandini/Previsor-do-Tempo/blob/master/Imagens/Previsor%20do%20Tempo%20-%20LCD.JPG)

# Componentes
**Arduino**

O Arduino Uno foi o microcontrolador escolhido para utilização neste projeto.

![Arduino UNO.](https://github.com/alanbrandini/Previsor-do-Tempo/blob/master/Imagens/Arduino%20Uno.jpg)

**LCD**

O LCD _Grove LCD RGB Backlight_ foi utilizado no projeto para mostrar o nome da cidade em questao (e.g., "São Carlos"), a temperatura nesta cidade (e.g., "Temp: 22C"), o _status_ da conexão com a rede ("Conectando..." ou "Conectado!"). Além disso, sua cor de fundo varia com a temperatura da cidade (e.g., se em Aracaju estiver 36°C, o LCD ficará vermelho; se em Curitiba estiver 2°C, o LCD ficará azul). Ainda, enquanto o sistema tenta conectar-se ao servidor, o LCD fica vermelho e, ao conectar-se, torna-se verde.
Para a ligação do LCD no Arduino, foi utilizado um _shield_ base da Groove, sendo a conexão realizada pelos pinos I2C.

![Grove LCD RGB Backlight.](https://github.com/alanbrandini/Previsor-do-Tempo/blob/master/Imagens/Grove-LCD%20RGB%20Backlight.jpg)
![Grove Base Shield para o Arduino.](https://github.com/alanbrandini/Previsor-do-Tempo/blob/master/Imagens/Grove%20Base%20Shield%20for%20Arduino.jpg)

**_Stepper_**

O motor de passo (_stepper_) é responsável pela abertura e fechamento da janela em caso de chuva. A API de previsão do tempo (_API Yahoo Weather_) manda, juntamente com a temperatura corrente, um código de condição do tempo. Com isso, pode-se definir quais códigos representam chuva.
Caso seja detectada chuva nesse código, o motor é acionado e a janela fecha. Quando a chuva para, o motor abre a mesma. A fim de evitar que o motor seja forçado, a janela é fechada somente se não estivesse chovendo na última consulta e é aberta somente se não está chovendo e estava durante a consulta anterior.

![Motor de passo (Stepper).](https://github.com/alanbrandini/Previsor-do-Tempo/blob/master/Imagens/Stepper.JPG)

**CI ULN2003A**

Este componente é utilizado para controlar o motor de passo, juntamente com a biblioteca _Stepper.h_.

![CI ULN2003A para controle do motor de passo.](https://github.com/alanbrandini/Previsor-do-Tempo/blob/master/Imagens/ULN2003A.jpeg)

**Sensor de Presença**

O sistema só busca a previsão na API quando o sensor de presença estiver em nível alto, isto é, quando detectar presença.

![Sensor de presença PIR.](https://github.com/alanbrandini/Previsor-do-Tempo/blob/master/Imagens/Sensor%20de%20presen%C3%A7a%20PIR.jpg)

**Botão**

Ao apertar o botão, o circuito muda de cidade e passa a apresentar outra cidade escrita no _display_, enquanto o sistema passa a buscar a previssão do tempo para a cidade apresentada.

![Botão.](https://github.com/alanbrandini/Previsor-do-Tempo/blob/master/Imagens/Pushbutton.jpg)

**_Shield Ethernet_**

O _shield ethernet_ é responsável pela conexão do sistema com o servidor _backend_ e utilização da _API Yahoo Weather_ para a temperatura no local e o código de condição do tempo.

![Shield Ethernet.](https://github.com/alanbrandini/Previsor-do-Tempo/blob/master/Imagens/Ehernet%20Shield.jpg)

# Implementação

Primeiramente foi usada uma biblioteca chamada "RestClient for Arduino" disponível pelo link: https://github.com/csquared/arduino-restclient. Nesta biblioteca usamos funções que facilitaram o uso do shield ethernet para conectar de maneira simples o arduino com uma API do yahoo que mostrava informações a respeito do tempo e do clima de uma cidade (São Carlos para o nosso caso).

Com as informações obtidas da API usamos o arduino para filtrá-las e exibir apenas a temperatura atual em um display LCD. Neste passo, foi necessário também uma outra biblioteca responsável pela comunicação entre o arduino e o LCD: https://github.com/Seeed-Studio/Grove_LCD_RGB_Backlight. Aproveitando que este é um LCD que pode ter sua luz de fundo alterada, para cada faixa de temperatura uma determinada cor é mostrada. Quanto mais alta a temperatura mais avermelhada é a cor e quanto mais próximo do 0ºC mais azul é a cor de fundo.

Além da temperatura, o sistema verifica se as condições climáticas informadas pela API indicam garoa, chuva, tempestade ou alguma condição relacionada. Em caso positivo o motor de passos é acionado para representar uma janela que se fecha, impedindo que a água da chuva entre naquele ambiente.

Após isto, colocamos um sensor de presença, que ao detectar movimento conecta-se novamente a API para verificar se houve alguma mudança nos dados coletados e repetir todos os passos descritos anteriormente. Além disso, um botão foi colocado para que toda vez que apertado uma nova cidade seja passada aos parâmetros da API, que buscaria as informações do clima e do tempo para esta cidade na próxima busca. 

# Evolução do projeto

Inicialmente, somente seria um motor movendo um ponteiro indicando que roupa utilizar ao sair de casa. Entretanto, com a facilidade da utilização do LCD e do Shield I2C, evoluiu-se o projeto a fim de aumentar o número de informações apresentadas e de funcionalidades: o motor passou a abrir e fechar a janela e o LCD apresenta temperaturas para diferentes (3) cidades, bem como o _status_ da conexão com a API.

O desenvolvimento do projeto é realizado pelos alunos **Alan R. da S. Brandini** e **Gabriel T. Caschera**, alunos do curso de **Engenharia de Computação (DC/UFSCar)**.
