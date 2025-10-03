# ECU-PIVOT-ESB
Projeto Automasensor: ECU-PIVOT-ESB <br>
Projeto Pivot na placa ESB

# Entradas Digitais
IN1: Fim de curso Direita (Retenção) <br>
IN2: Fim de curso Esquerda (Retenção) <br>
IN3: Segurança 1 (Retenção) <br>
IN4: Segurança 2 (Retenção) <br>
IN5: Seleção modo de trabalho (Pulso)<br>
IN6: Botoeira Avanço (Direita) (Pulso)<br>

# Entradas Analógicas
AN1: Transdutor de pressão. 4-20mA / 0-10bar <br>

# Relés
K1: Contato Auxiliar <br>
K2: Bomba Booster <br>
K3: Sentido Reversão <br>
K4: Sentido Direto <br>
K5: Percentímetro <br>
K6: Bomba Principal <br>
K7: Válvula da Bomba Principal <br>
K8: Ferti Irrigação <br>
K9: Relé Partida <br>
K10: Relé Parada <br>

# Descritivo das Saídas
K1: Fica acionando durante a operação, quando máquina parada fica desligado <br>
K2: Aciona durante a irrigação. Caso entre em uma área de obstáculos, desliga. Caso o equipamento não tenha geo referenciamento, desliga. <br>
K3 e K4: Acionam conforme sentido (reverso ou direto) <br>
K5: Fica acionado no modo de deslocamento a seco e tem o seu acionamento intervalado (conforme configurado) no modo de irrigação. <br>
K6: Acionado no modo de irrigação e nunca aciona no deslocamento a seco <br>
K7: Possui o mesmo acionamento do K6, funciona totalmente em paralelo <br>
K8: Pode acionar (conforme configuração) no modo irrigação, deslocamento a seco sempre desligado <br>
K9: Aciona quando a máquina deve "andar" serve em qualquer modo de funcionamento. Funciona com um pulso de 3s, após esse tempo volta a desligar <br>
K10: Aciona quando a máquina deve "parar" serve em qualquer modo de funcionamento <br>

# Escopo
A placa possui dois modo de trabalho o MODO DESLOCAMENTO A SECO e o modo IRRIGAÇÃO. <br>
Quando necessário inverter o sentido K3 x K4, o equipamento deve inicialmente desligar o relé do sentido anterior e aguardar 1,5s para acionar o relé do novo sentido.
<br>
O equipamento possui uma janela de horário de "ponta", dentro dessa janela o equipamento não pode acionar, devido a pico de energia e multa da concessionária fornecedora. <br>
O equipamento realiza reportes pela internet em um servidor via HTTP, para o APP identificar qual placa se refere o reporte, o equipamento possui um número serial editável. <br>
O equipamento possui um horímetro, que conta as horas de máquina operando, seja ela em deslocamento a seco ou no modo de irrigação. Só não serão consideradas horas em que a máquina esteja efetivamente parada. Obs: O período em que a máquina está aguardando pressurização no modo de irrigação conta como máquina rodando. <br>
<br>
No modo de Deslocamento a seco: <br>

1) O equipamento fica parado até ser selecionado o lado do deslocamento, através das botoeiras ou através do app. O deslocamento não para, até bater no fim de curso ou ser comandado o desligamento. É possível configurar uma posição HOME, que faz com que o deslocamento a seco pare nessa posição (quando comandado para parar em HOME).
2) A saída do percentímetro fica acionada até a máquina desligar. <br>
3) Aciona o auxiliar (K1) e igorna AN1 <br>
4) O equipamento somente monitora a falha de segurança 2 segundos após iniciar o deslocamento.
<br>
No modo de Irrigação: <br>

1) Fica parado até selecionar o lado. <br>
2) Ao selecionar o lado entra no modo de pressurização, onde a bomba (K6) e o auxilair (K1) são acionados. <br>
3) O equipamento aguarda a pressão (AN1) ultrapassar o valor configurado em alarme e então começa o deslocamento. <br>
O tempo a ser aguardado é configurado nos menus. Caso o equipamento perca pressão após já ter atingido, volta ao estado de pressurização, parando a máquina e aguarda a pressão retornar. O tempo não é reiniciado. <br>
4) Caso o tempo de aguarde da pressão zere, o equiamento para por falta de pressão. Caso falte pressão após o tempo se esgotar o equipamento não retorna mais ao estado de pressuriazação e também para por falta de pressão. <br>
5) Após a pressão se estabilizar, acionam: Ferti Irrigação (K8) (se configurado), Bomba Booster (K2) (caso fora da zona de obstáculos). <br>
6) O percentímetro (K5) aciona por X segundos e desliga por X segundos. A soma dos dois tempos (ON + OFF) é configurado nos menus. No menu da lâmina d'água o operador coloca o valor em mm. Esse valor é comparado com uma tabela configurada pelo usuário. Nessa tabela indica o valor em percentual e mm. O sistema calcula qual o percentual conforme a tabela. Com esse percentual o sistema busca o tempo de ligado. <br>
7) A bomba booster fica acionada no modo irrigação e desliga nesse modo apenas quando passa por uma área de obstáculos. Programada pelo operador. A distância para esses obstáculos também é programada. Caso o equipamento perca o sinal do GPS, desliga a bomba booster. <br>
8) Ao chegar no fim de curso para em caso de modo 1 ciclo OU continua no sentido reverso, em caso de modo contínuo. Esse modo é programado no menu de configurações. <br>
9) O equipamento somente monitora a falha de segurança 2 segundos após iniciar o deslocamento, não monitora durante a fase de pressurização. <br>

# Emergências e Falhas
IN3 e IN4 funcionma como indicador de segurança (NF), ao abrir também para todo o sistema. Porém somente são monitorados enquanto a máquina está em movimento. <br>
AN1 é monitorado apenas no modo irrigação, deve parar caso o sistema indique baixa pressão. Caso o sistema esteja dentro do tempo de pressurização, caso a pressão baixe do valor máximo, retorna a fase de pressurização. <br>

# Servidor
Há 3 endpoints para acesso de informações. Um com método POST para reportar o status atual do equipamento e dois com o método GET, um para receber um acionamento e outro para receber configurações. As intruções são no formato json e tanto o reporte quando o acionamento e configurações possum um campo com o mesmo formato dos antigos arquivos do FTP, a diferença é que no lugar de um new line agora se tem um espaço. <br>

# Protocolo RS485 GNSS
Pivot: $,030,1,\r\n <br>
GNSS: $,030,1,hhmmssDDMMAA,ggmm.sssss,S,gggmm.sssss,W,\r\n <br>

# Protocolo Lora Gateway
O rádio Lora utilizado é o que fica na placa soquete, não é o mesmo do GNSS. <br>
O endereço e canal devem ser diferentes do Lora do GNSS. <br>
O endereço do gateway é 2 acima do endereço do pivot. <br>
Para gravar o endereço no gateway deve se acessar via UBS no gateway. <br>
Comandos: <br>
Comando 1: $,01,POSICAO ATUAL GPS, qualdiade Lora (GNSS),\r\n <br>
Comando 2: $,02,HORA/DATA, OPERACAO SENTIDO RETORNO TEMPO PRESSURIZACAO PRESSAO STATUS PORTA, NUMERO SERIAL,\r\n <br>
Comando 3: $,03,ALARMES,HORIMETRO,HORARIO PONTA OBSTACULO ENCONTRADO,\r\n <br>
Comando 4: $,04,POSICAO INICIO OPERACAO GPS, DADO CONFIAVEL (INICIO OPERACAO),\r\n <br>
Nenhum desses comandos obterá resposta do gateway <br>
Comando 5: $,05,\r\n <br>
Resposta: $,05,DADO VALIDO,ID ACIONAMENTO,OPERACAO SENTIDO RETORNO PIVOT FERTI BOMBA, LAMINA DAGUA,\r\n <br>

# Status LEDs
O LED CPU (Verde) pisca a cada 500ms. <br>
<br>
O LED COM (Amarelo) pisca toda vez que um pacote é recebido da ECU GPS. <br>
