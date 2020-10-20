struct bit16ErrorVec{
    uint16_t nOfErrors = 0;
    bool errorVec[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
}typedef bit16ErrorVec;


uint16_t injectErrorInChunk(uint16_t chunk, uint16_t microPosition){
    uint16_t auxiliary = pow(2,microPosition);
    return (chunk ^ auxiliary);
}

uint16_t* pureRandomModel(uint16_t* packet, uint32_t errorQuantity = 1){

    bit16ErrorVec packageErrors[packet[2]/2];
    if(errorQuantity > (packet[2])*8) return nullptr;
    uint16_t position, microPosition;
    uint16_t limit = 30, length = packet[2];
    for(uint16_t i=0; i<errorQuantity; ++i){
        /*
        Esse for (abaixo) serve pra sortear um chunk no pacote para sofrer um erro 
        Sim, parece mais complicado do que deveria ser, mas isso é pra garantir que apos tentar escolher uma
        cadeia de 16 bits que tem menos de 16 alteraçoes um determinado numero de vezes e falhar, o algoritmo escolhe a primeira
        palavra de 16 bits que ainda tem bits que nao foram alterados
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
        ++packageErrors[position].nOfErrors;
        packageErrors[position].errorVec[microPosition] = true;
        //cout << position << " : " << microPosition << endl;
        packet[position] = injectErrorInChunk(packet[position], microPosition);

    }
    return packet;
}

//The following implementations are based in the description given in this paper
//https://www.cister.isep.ipp.pt/docs/bit_error_models/369/view.pdf
uint16_t* gilbertElliotModel(uint16_t* packet, uint16_t Tb, uint16_t Tg){
	//Tb and Tg stands for the mean duration of the bad burst and the good burst respectively. Tb= mean of the length of error bursts
	double Pbb = Tb/(Tg+Tb);
	double Pgb = 1 - Pbb;
	uint16_t length = packet[2]*8;
	bool errorVec[length] = {0};

	//Considerei o bit da posição -1 (que não existe) como correto
	errorVec[0] = trueFalseProb(Pgb);
	//Cada bit percorrido se baseia no bit anterior para calcular a probabilidade de ativar o bit i do vetor de erros
	for(uint32_t i=1; i<length; ++i){
		if(errorVec[i-1] == false){
			errorVec[i] = trueFalseProb(Pbb);
		}else{
			errorVec[i] = trueFalseProb(Pgb);
		}
	}

	//Injeta os erros de acordo com o errorVec. Importante lembrar que bit 0 do primeiro pacote é representado pelo errorVec[15] e o bit 15 pelo errorVec[0]
	for(uint16_t i=0; i<(length/16); ++i){
		uint32_t posInError = i*16;
		for(uint16_t j=15; j>=0; --j){
			if(errorVec[posInError + j] == true){
				packet[i] = injectErrorInChunk(packet[i], 15-j);
			}
		}
	}
	return packet;
}