# dama

Nome: Kaio Vinicius Souza Santos
RA:11201810069

Jogo de dama desenvolvido em ImGui

 A aplicação trata-se de um jogo de dama, este jogo foi desenvolvido pensando em um jogo de damas convencional, onde de um lado temos as peças de formato X e do outro as peças do formato O.
 No inicio do jogo as peças de formato X sempre começam a jogar, e o método de jogo é igual ao jogo normal de dama, o primeiro jogador move uma peça dele e no próximo turno o próximo jogador move uma peça dele, o jogo irá orientar caso seja possível "comer" uma peça inimiga.
 
 TO DO
 
  - O jogo do jeito que está não está 100% apesar de permitir que algum dos lados vença acontece que, por falta de tempo, não foi possível implementar o caso em que a peça chega ao final do tabuleiro e ganha o poder de voltar e avançar mais casas que o normal.

Desenvolvimento da Aplicação:

 A aplicação foi desenvolvida utilizando um array de char (m_board) onde inicialmente setamos seu tamanho inicial para 8, como convencionalmente temos em um jogo de dama. Este array contém a posição de todas as peças X e O dentro do tabuleiro e quando adicionarmos um item dentro desse vetor o mesmo irá aparecer na tabela.
 O jogo basea-se em turnos, portanto ao mover uma peça o turno automaticamente passa para o próximo jogador através de uma variável (m_Xturns).
 Ao mover uma peça o jogo irá avaliar quais são as casas possíveis que a peça pode se mover, sendo possível apenas mover-se lateralmente em um sentido e caso não tenha peças nas posições que deseja colocar a peça.
 Quando você tenta mover uma peça que pode comer outra peça inimiga o jogo irá avaliar se você pode realizar esse movimento, primeiro ele avalia se você pode comer lateralmente a peça avaliando se a posição final da peça após você comer se encontra dentro de uma posição valida do tabuleiro, caso sim ele irá avaliar se a posição final da peça após comer o inimigo não está ocupada por ninguém, caso essas duas condições sejam preenchidas então será possível comer a peça inimiga.
 No final quando algum dos jogadores comer todas as peças inimigas então será definido o campe
