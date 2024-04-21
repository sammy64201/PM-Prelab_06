/*
 * Pre_lab_06.c
 *
 * Created: 4/20/2024 6:03:55 PM
 * Author : samue
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void setup(void);
void initUART9600(void);  // significa que el UART comenzara a 9600
void writeUART(char Caracter); //  funcion para mandar caracter por caracter
void WriteTextUART(char * Texto); // funcion para enviar un texto completo de un solo 


// declaracion de variables volatiles
volatile uint8_t bufferTX;
volatile uint8_t bufferRX;


void setup(){
	// declaro todo el puerto B como salida 
	DDRB |= (1<<DDB0) | (1<<DDB1) | (1<<DDB2) | (1<<DDB3) | (1<<DDB4) | (1<<DDB5);
	initUART9600();
	sei();
}	

void UART_mensaje(){
	/*writeUART('H');
	writeUART('O');
	writeUART('L');
	writeUART('A');
	writeUART(' ');
	writeUART('M');
	writeUART('U');
	writeUART('N');
	writeUART('D');
	writeUART('O');
	writeUART('\n');
	writeUART(10);
	writeUART(13);*/
	
	WriteTextUART("Hola mundo con la otra funcion ;) ");
	writeUART(10);
	writeUART(13);
	
}

int main(void)
{
    setup();
	UART_mensaje();
	
	
    while (1) 
    {
		recieveUART();
		PORTB = bufferRX;
    }
}

void initUART9600(void){
	// paso 1: establecer pines como entrada y salida 
	//RX COMO ENTRADA Y TX COMO SALIDA 
	DDRD &= ~(1<<DDD0);
	DDRD |= (1<<DDD1);
	
	// PASO 2: configurar UCSR0A
	UCSR0A = 0;
	
	// PASO 3: configurar UCSR0B  DONDE SE HABILITA LA ISR DE RECEPCION 
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);

	//PASO 4: CONFIGURAR UCSR0C
	UCSR0C = 0;
	//CONFIGURAR ASINCORONO Y SIN PARIDAD, 1 BIT DE STOP, 8 CARACTERES
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	//PASO 5: SE CALCULA EL VALOR DE BAUD Y VELOCIDAD: 9600
	UBRR0 = 103;
		
}


void writeUART(char Caracter){
	// si no es 1 quedate esperando POR LO TANTO HASTA QUE SEA 1 VA CONTINUAR 
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = Caracter;
	
}

void WriteTextUART(char * Texto){
	uint8_t i;
	for (i=0; Texto[i]!= '\0'; i++){
		while (!(UCSR0A & (1<<UDRE0)) );
		UDR0 = Texto[i];
	}
}

void recieveUART(){
	if (UCSR0A & (1<<RXC0)){
		bufferRX = UDR0;
	}
}

ISR(USART_RX_vect){
	
	bufferTX = UDR0;
	//while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = bufferTX;
	
}