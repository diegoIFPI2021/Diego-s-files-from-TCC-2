const byte encoderA_1 = 2;          // Sinal A do encoder
const byte encoderB_1 = 3;          // Sinal B do Encoder
const byte encoderA_2 = 18;
const byte encoderB_2 = 19;
//const float pi = 3.1416; //Valor constante de pi para divisões
volatile int ValEncoder_1 = 0;              //  Variável que conta a quandidade de pulsos do encoder
volatile int ValEncoder_2 = 0;
volatile boolean readingA, readingB;      //  Variáveis que determinam o estado atual do encoder para determinar o sentido de rotação
volatile boolean readingA_2, readingB_2;
char string;
volatile int angulo_1[500];
volatile int angulo_2[500];
unsigned volatile int tempo_1[500]; // os tempos serão os mesmos para ambos os pendulos
unsigned volatile int tempo_2[500];
unsigned volatile int e_1 = 0;
unsigned volatile int e_2 = 0;
const int num_pontos_1 = 500;
const int num_pontos_2 = 500;


void Interrupt_A_1() {
  readingA = digitalRead (encoderA_1);
}

void Interrupt_B_1() {
  readingB = digitalRead (encoderB_1);
  if (readingA != readingB) {
    ++ValEncoder_1;
  }
  if (readingA == readingB) {
    --ValEncoder_1;
  }

  tempo_1 [e_1] = millis();
  angulo_1 [e_1] = ValEncoder_1;
  ++e_1;
}



void Interrupt_A_2() {
  readingA_2 = digitalRead (encoderA_2);
}

void Interrupt_B_2() {
  readingB_2 = digitalRead (encoderB_2);

  if (readingA_2 != readingB_2) {
    ++ValEncoder_2;
  }
  if (readingA_2 == readingB_2) {
    --ValEncoder_2;
  }

  tempo_2 [e_2] = millis();
  angulo_2 [e_2] = ValEncoder_2;
  ++e_2;

}

//Executar uma função em intervalos de tempos regulares.


void setup() {
  //configurações movidas

  pinMode(encoderA_2, INPUT_PULLUP); // NECESSÁRIO PARA QUE OS PINOS NÃO FIQUEM COM VALORES FLUTUANDO GREEN
  pinMode(encoderB_2, INPUT_PULLUP); // WHITE

  pinMode(encoderA_1, INPUT_PULLUP); // NECESSÁRIO PARA QUE OS PINOS NÃO FIQUEM COM VALORES FLUTUANDO GREEN
  pinMode(encoderB_1, INPUT_PULLUP); // WHITE

  Serial.begin(2000000);
  
  
}




void transfere_dados_encoder_1 () {

  Serial.println ("---------------------------------- ");
  Serial.println ("        TRANSFERINDO DADOS         ");
  Serial.println ("            ENCODER 1              ");
  Serial.println ("---------------------------------- "); 
  delay(200);



  for (int i = 0 ; i < num_pontos_1; ++i) {
    //Serial.print(angulo_1[i]*(0.15)); // Este valor foi tomado considerando as 2400 divisões. Este angulo está em graus. O 
    Serial.print(angulo_1[i]); //Os dados impressos serão dados brutos. No origin será feito o cálculo por meio de uma função pré-definida.
    Serial.println(" , ");
   Serial.println(tempo_1[i]);
  }
}





void transfere_dados_encoder_2 () {
  Serial.println ("---------------------------------- ");
  Serial.println ("        TRANSFERINDO DADOS         ");
  Serial.println ("            ENCODER 2             ");
  Serial.println ("---------------------------------- "); 
  delay(200);

  for (int i = 0 ; i < num_pontos_2; ++i) {
    //Serial.print(angulo_2[i]*(0.15)); // Este valor foi tomado considerando as 2400 divisões. Este angulo está em graus.
    Serial.print(angulo_2[i]);
    Serial.print(" , ");
    Serial.println(tempo_2[i]);
  }
}

void inicia_experimento() {
  // configura os pinos as interrupções para incializar o experimento
//  Serial.println("**********************");
  Serial.println(" EXPERIMENTO INICIADO ");
  //Serial.println("**********************");

  attachInterrupt (digitalPinToInterrupt(2), Interrupt_A_1, CHANGE); // CHAMA A INTERRUPÇÃO QUANDO HÁ VARIAÇÃO NO ESTADO LÓGICO (HIGH OU LOW) DO PIN 2 OU 3
  attachInterrupt (digitalPinToInterrupt(3), Interrupt_B_1, CHANGE);

  attachInterrupt (digitalPinToInterrupt(18), Interrupt_A_2, CHANGE); // CHAMA A INTERRUPÇÃO QUANDO HÁ VARIAÇÃO NO ESTADO LÓGICO (HIGH OU LOW) DO PIN 2 OU 3
  attachInterrupt (digitalPinToInterrupt(19), Interrupt_B_2, CHANGE);

}


void loop() {

  if ((e_1 == 0 ) && (e_2 == 0)) { //&& indicam se ambos os operandos são verdadeiros
    Serial.flush();
    delay(100);
    Serial.println("  ========  Pêndulo Acoplado  =======  ");
    //Serial.println("        Por favor, selecione uma opção       ");
    Serial.println("        1 - Iniciar Experimento       ");

    int b = 0;
    while (!Serial.available()) {
      // Aguarda o usuário digitar alguma opção
      ++b;
    }

    string = Serial.read();

    if (string == '1') {
      inicia_experimento(); //habilita as interrupções e libera para realizar a medição
      int a = 0;
      while ((e_1 <= num_pontos_1) && (e_2 <= num_pontos_2)) {
        ++a;
      }
    }
  }

  
  if (e_1 >= num_pontos_1) {
    detachInterrupt (digitalPinToInterrupt(2)); // CHAMA A INTERRUPÇÃO QUANDO HÁ VARIAÇÃO NO ESTADO LÓGICO (HIGH OU LOW) DO PIN 2 OU 3
    detachInterrupt (digitalPinToInterrupt(3));
    transfere_dados_encoder_1();
    e_1 = 0;
  }

  if (e_2 >= num_pontos_2) {
    detachInterrupt (digitalPinToInterrupt(18)); // CHAMA A INTERRUPÇÃO QUANDO HÁ VARIAÇÃO NO ESTADO LÓGICO (HIGH OU LOW) DO PIN 2 OU 3
    detachInterrupt (digitalPinToInterrupt(19));
    transfere_dados_encoder_2();
    e_2 = 0;

  }


}
