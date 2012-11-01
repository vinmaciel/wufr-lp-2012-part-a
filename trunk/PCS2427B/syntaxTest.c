/*	Observacoes gerais:
 *		a variavel i eh usada em dois escopos diferentes no mesmo nivel
 *		a variavel string eh usada em tres escopos diferentes, dois deles no mesmo nivel
 *		caracteres e inteiros sao tratados da mesma forma
 */

/* Bibliotecas a serem importadas (generica, para teste de sintaxe) */
import "biblioteca.h";

/* Declaracao das variaveis no escopo global */
int var0, var1, var2;
int vet0[4], vet1[2][2];
bool boolean;

/* Declaracao de funcoes (prototipos) com ou sem parametros */
void leMensagemDoSistema(char string[16]);
void imprimeMensagemDeErro();
bool ehMultiplo(int base, int multiplo);
int tamanho(char string[16]);

/* Descricao da funcao principal, sem parametro */
void main() {
	/* Variaveis em escopo em nivel 0 */
	int tam, linha[2];
	char aux;

	/* Rotulo */
	begin:

	/* Chamada de subrotina com parametro */
	leMensagemDoSistema(string);

	/* Atribuicao, chamada de subrotina com parametro */
	tam = tamanho(string);

	/* Comando de decisao multiplca (com default) */
	switch(tam) {
	/* Rotulos de case */
	case 0:
	case 3:
	case 5:
		/* Comando de desvio (saida do programa principal) */
		return;

	/* Rotulos de case */
	case 1:
		/* Atribuicao */
		aux = string[0];
		/* Comando de desvio (saida de bloco) */
		break;

	case 2:
		/* Atribuicao em vetor */
		linha[0] = string[0];
		linha[1] = string[1];
		/* Comando de desvio (saida de bloco) */
		break;

	/* Rotulos de case */
	case 4:
		/* Atribuicao em matriz */
		vet1[0][0] = string[0];
		vet1[0][1] = string[1];
		vet1[1][0] = string[2];
		vet1[1][1] = string[3];
		/* Comando de desvio (saida de bloco) */
		break;

	/* Default action */
	default:
		/* Atribuicao */
		var0 = string[0];
		var1 = string[1];
		var2 = string[2];

		/* Comando de iteracao, condicao de maior-ou-igual */
		while(tam >= 3) {
			/* Comando de decisao simples (apenas IF) */
			/* Chamada de subrotina */
			if(ehMultiplo(string[tam-1], var2)) {
				/* Comando de desvio incondicional */
				goto end;
			}
			/* Atribuicao */
			string[tam-1] = var1;

			/* Comando de decisao simples (apenas IF) */
			/* Condicao de maior */
			if(string[tam-2] > string[tam-1]) {
				/* Comando de desvio (retorno ao teste de iteracao) */
				continue;
			}

			/* Decremento */
			tam--;
		}
	}

	/* Comando de desvio incondicional */
	goto begin;

	/* Rotulo */
	end:
}

/* Descricao de procedimento com parametro */
void leMensagemDoSistema(char string[16]) {
	/* Variavel em escopo em nivel 1 */
	int i;

	/* Laco iterativo */
	/* Atribuicao, condicao de menor E diferenca, incremento */
	for(i = 0; i < 16 && string[i-1] != '\n'; i++) {
		/* Comando de leitura */
		string[i] << scan();
	}

	/* Atribuicao em um vetor */
	string[i-1] = '\0';
}

/* Descricao de procedimento sem parametro */
void imprimeMensagemDeErro() {
	/* Comando de escrita */
	print() >> "ERRO";
	/* Chamada de operacao do sistema operacional */
	system << "exit";
}

/* Descricao de funcao com parametros */
bool ehMultiplo(int base, int multiplo) {
	/* Comando de decisao simples (apenas IF) */
	/* Condicao de igualdade */
	if(multiplo%base == 0) {
		/* Comando de desvio (saida de subrotina) */
		return 1;
	}

	/* Comando de desvio (saida de subrotina) */
	return 0;
}

/* Descricao de funcao com parametro */
int tamanho(char string[16]) {
	/* Variaveis em escopo em nivel 1 */
	int i, tam;

	/* Laco iterativo */
	/* Atribuicao, condicao de menor, incremento */
	for(i = 0; i < 16; i++) {
		/* Comando de decisao simples (IF e ELSE) */
		if(string[i] == '\0') {
			/* Comando de desvio (saida de bloco) */
			break;
		}
		else {
			/* Expressao unaria (incremento) */
			tam++;
		}
	}

	/* Comando de decisao simples (apenas IF) */
	/* Condicao de igualdade */
	if(i == 16) {
		/* Chamada de subrotina sem parametros */
		imprimeMensagemDeErro();
	}

	/* Comando de desvio (saida de subrotina) */
	return tam+1;
}
