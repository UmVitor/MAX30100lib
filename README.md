## Biblioteca MAX30100 - Oximetro de pulso com medição diferencial

O presente é projeto é uma alteração da biblioteca [MAX30100](https://github.com/oxullo/Arduino-MAX30100/) que foi realizada para comportar dois sensores e uma medição diferencial, as seguintes classes foram modificadas:

* PulseOximeter
* SpO2Calculator
* MAX30100

Além disso, a classe ```interceptor``` foi adicionada para manejar os sinais recebidos pelos sensoeres. Na figura abaixo, é possível observar o fluxo das classes que o código passa até o valor final da saturação de oxigênio.

![imagem_classe](/FluxoClasses.png)

O código que utiliza essa biblioteca, está nesse mesmo repositorio e se chama: [OximetroPulson.ino](/OximetroPulso.ino)

