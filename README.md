# Implementação da heurística do Travelling Sales Person (Caixeiro Viajante) utilizando programação dinâmica.

## Descrição do problema

O Problema do Caixeiro Viajante (PCV) ou Travelling Sales Person (TSP) é um problema que tenta determinar a menor rota para percorrer uma série de cidades (visitando uma única vez cada uma delas), retornando à cidade de origem. Ele é um problema de otimização NP-difícil inspirado na necessidade dos vendedores em realizar entregas em diversos locais (as cidades) percorrendo o menor caminho possível, reduzindo o tempo necessário para a viagem e os possíveis custos com transporte e combustível.

## Heurística utilizada para a implementação

Neste [arquivo]( https://github.com/RodolfoHerman/caixeiro-viajante-com-programacao-dinamica/blob/master/heuristicaTSP.pdf) contém a heurística base para a implementação, e este outro [arquivo]( http://www.cs.cmu.edu/~rweba/algf09/dynprog.pdf) o documento original. Também foi utilizado o seguinte [vídeo](https://www.youtube.com/watch?v=aQB_Y9D5pdw) para complementar o entendimento da heurística. 
A heurística foi implementada no modelo de programação dinâmica. Ela é aplicável a problemas nos quais a solução ótima pode ser computada a partir da solução ótima previamente calculada e memorizada - de forma a evitar recálculo - de outros subproblemas que, sobrepostos, compõem o problema original.

## Utilização da heurística implementada

Os testes foram realizados num intervalo de 11 até 16 cidades, este intervalo foi escolhido por motivos de tempo, pois para o número de cidades menores do que 11 o tempo é baixo e para o número de cidades igual ou abaixo de 16 o tempo é mais alto (cerca de 10 minutos). Ao tentar executar o programa para 19 cidades o algoritmo não terminou, sendo que o tempo de espera foi de 1 hora e 30 minutos aproximadamente.

O arquivo Cidades.txt possui o arquivo para testes. O primeiro número represente a quantidade de cidades o restante dos números representam as coordenadas de cada cidade.
