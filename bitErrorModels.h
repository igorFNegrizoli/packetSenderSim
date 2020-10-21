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
uint16_t* gilbertElliotModel(uint16_t* packet, uint16_t TbAux, uint16_t TgAux){
	//Tb and Tg stands for the mean duration of the bad burst and the good burst respectively. Tb= mean of the length of error bursts
	double Tb = TbAux, Tg = TgAux;
	double Pgg = Tg/(Tg+Tb);
	double Pbb = Tb/(Tg+Tb);
	uint16_t length = packet[2];
	bool state = true;//good state
	//cout << Pgg << "  " << Pbb << endl;
	for(uint32_t i=0; i<length/2; ++i){
		for(uint16_t pos=15; pos<16; --pos){
			if(state == true){
				if(trueFalseProb(Pgg)){
					state = true;
				}else{
					packet[i] = injectErrorInChunk(packet[i], pos);
					cout << i << " : " << pos << endl;
					state = false;
				}
			}else{
				if(trueFalseProb(Pbb)){
					packet[i] = injectErrorInChunk(packet[i], pos);
					cout << i << " : " << pos << endl;
					state = false;
				}else{
					state = true;
				}
			}
		}
	}
	
	return packet;
}

uint16_t* bernoulliModel(uint16_t* packet, double BER){
	uint16_t length = packet[2], errCount = 0;
	double Perr = 1 - pow((1-BER),length*8);
	cout << Perr << endl;
	for(uint16_t i = 0; i<length/2; ++i){
		for(uint16_t j = 0; j<16; ++j){
			if(trueFalseProb(Perr) == true){
				packet[i] = injectErrorInChunk(packet[i], j);
				++errCount;
			}
		}
	}
	cout << errCount << " errors occurred" << endl;
	return packet;	
}

