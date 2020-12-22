#include "toolbox.h"

//Provavelmente nao precisarei mais dessa struct futuramente, nos proximos commits devo deletar
struct bit16ErrorVec{
    uint16_t nOfErrors = 0;
    bool errorVec[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
}typedef bit16ErrorVec;

//Flippa o bit na posição "microPosition" da palavra "chunk" 
//Flips the "microPosition" bit of "chunk"
uint16_t injectErrorInChunk(uint16_t chunk, uint16_t microPosition){
    uint16_t auxiliary = pow(2,microPosition);
    return (chunk ^ auxiliary);
}

//Injeta "errorQuantity" erros no pacote em posições aleatórias
//Inject "errorQuantity" errors in random positions of packet
uint16_t* pureRandomModel(uint16_t* packet, uint32_t errorQuantity = 1, uint16_t limit = 30){

    bit16ErrorVec packageErrors[packet[2]/2];
    if(errorQuantity > (packet[2])*8) return nullptr;
    uint16_t position, microPosition;
    uint16_t length = packet[2];
    for(uint16_t i=0; i<errorQuantity; ++i){
        /*
        Esse for (abaixo) serve pra sortear um chunk(palavra de 16b) no pacote para sofrer um erro 
        Sim, parece mais complicado do que deveria ser, mas isso é pra garantir que apos tentar escolher uma
        cadeia de 16 bits que tem menos de 16 alteraçoes um determinado numero de vezes (limit) e falhar, o algoritmo escolhe a primeira
        palavra de 16 bits que ainda tem bits que nao foram alterados

        The below "for" is made for picking one chunk(16b word) in packet to inject an error
        Yes, it looks more complicated than it should be, but it is here to make sure that after choosing a chunk that has less than
        16 error injections a determined number of times (limit) and fail, the algorithm chooses the first 16b word that still has
        non-flipped bits
        */
        for(uint16_t j = 0; j<=limit; ++j){
            position = randomnum(length/2);
            if(packageErrors[position].nOfErrors < 16) break;
            if(j == limit){
                for(uint16_t k = 0; k<(length/2); ++k){
                    if(packageErrors[k].nOfErrors < 16){
                        position = k;
                        break;
                    }
                }
            }
        }
        //Fiz a mesma coisa que descrevi no comentario anterior mas para escolher qual bit sera alterado em vez da palavra
        //Did the same thing that I described in the previous comment but this is made to chose the bit instead of the 16b word
        for(uint16_t j = 0; j<=limit; ++j){
            microPosition = randomnum(16);
            if(packageErrors[position].errorVec[microPosition] == 0) break;
            if(j == limit){
                for(uint16_t k = 0; k<16; ++k){
                    if(packageErrors[position].errorVec[microPosition] == 0){
                        microPosition = k;
                        break;
                    }
                }
            }
        }
        //Agora que as posiçoes foram escolhidas, chama-se a funçao que altera o bit
        //Now that the function chose the positions we call the function that inject the error
        ++packageErrors[position].nOfErrors;
        packageErrors[position].errorVec[microPosition] = true;
        //cout << position << " : " << microPosition << endl;
        packet[position] = injectErrorInChunk(packet[position], microPosition);

    }
    return packet;
}

//As seguintes implementações foram baseadas na descrição dada nesse paper //https://www.cister.isep.ipp.pt/docs/bit_error_models/369/view.pdf
//The following implementations are based in the description given in this paper //https://www.cister.isep.ipp.pt/docs/bit_error_models/369/view.pdf
uint16_t* simplifiedGilbertElliotModel(uint16_t* packet, uint16_t TbAux, uint16_t TgAux){
	//Tb e Tg significam respectivamente a media de duração da sequencia de bits ruins e bons
	//Tb and Tg stands for the mean duration of the bad sequence and the good sequence respectively
	double Tb = TbAux, Tg = TgAux;
	double Pgg = Tg/(Tg+Tb);
	double Pbb = Tb/(Tg+Tb);
	uint16_t length = packet[2];
	bool state = true;//good state
	for(uint32_t i=0; i<length/2; ++i){
		for(uint16_t pos=15; pos<16; --pos){
			if(state == true){
				if(trueFalseProb(Pgg)){
					state = true;
				}else{
					packet[i] = injectErrorInChunk(packet[i], pos);
					//cout << i << " : " << pos << endl;
					state = false;
				}
			}else{
				if(trueFalseProb(Pbb)){
					packet[i] = injectErrorInChunk(packet[i], pos);
					//cout << i << " : " << pos << endl;
					state = false;
				}else{
					state = true;
				}
			}
		}
	}
	return packet;
}

//Modelo de bernoulli descrito no artigo citado
//Bernoulli model mentioned in the said article
uint16_t* bernoulliModel(uint16_t* packet, double BER){
	uint16_t length = packet[2];
	for(uint16_t i = 0; i<length/2; ++i){
		for(uint16_t j = 0; j<16; ++j){
			if(trueFalseProb(BER) == true){
				packet[i] = injectErrorInChunk(packet[i], j);
			}
		}
	}
	return packet;	
}

//Funcionamento do algoritmo está descrito no item 4 do paper citado no comentario da implementação do modelo de Gilbert-Elliot
//How the algorithm works is described at the previous mentioned article
uint16_t* burstErrorPeriodicModel(uint16_t* packet, uint16_t Tmin, uint16_t Tmax, uint16_t Nmin, uint16_t Nmax){
	// Usar Tmin > 0 e Nmin > 0		Para um threshold(T) ou burstLength(N) de tamanho fixo, usar Tmin = Tmax e Nmin = Nmax
	// Use Tmin > 0 e Nmin > 0		For a fixed length threshold(T) or burstLength(N), use Tmin = Tmax and Nmin = Nmax
	uint16_t i, pos; //T -> threshold, N -> burst length
	uint16_t threshold, eLength;
	uint16_t pLen = packet[2], accumulator = 0;

	threshold = randomIntInterval(Tmin, Tmax);
	eLength = randomIntInterval(Nmin, Nmax);
	accumulator += threshold;

	for(i=0; i<pLen/2; ++i){
		for(pos=15; pos<16; --pos){
			if(eLength > 0){
				packet[i] = injectErrorInChunk(packet[i], pos);
				--eLength;
			}
			--threshold;

			if(threshold == 0){
				threshold = randomIntInterval(Tmin, Tmax);
				eLength = randomIntInterval(Nmin, Nmax);
				accumulator += threshold;
			}
		}
	}

	return packet;	
}