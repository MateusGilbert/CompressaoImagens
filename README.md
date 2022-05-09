# compressao_imagens
Estão armazenados neste repositório os programas que implementei para a disciplina de Compressão de Imagens no período 2022.1 da COPPE/UFRJ.

## Huffman
Programa para compressão/descompressão de arquivos utilizando código de Huffman.

## PPM
Os arquivos que implementam o compressor aritmético foram obtidos na disciplina. Programa implementa o modelo ppm utilizando como maior contexto o valor de 0 a 9, fornecido pelo usuário. Abaixo, exemplos de como exectutar a compressão/decompressão:

### ./encode -v -e \<origem\> 5 > \<destino\>

o arquivo origem é comprimido utilizando um contexto de comprimento máximo igual a 5, com as opções verbose (-v) e príncípio da exclusão (-e) selecionados;

### ./decode -v -e \<origem\> 5 > \<destino\>

decompressão, utilizando os mesmos parâmetros do exemplo anterior.

obs.: quando -v é selecionado, o programa escreve uma mensagem a cada 10k bytes processados para indicar que a compressão ainda está ocorrendo.

obs2.: quando -e é selecionado, o princípio da exclusão é utilizado na compressão. Isso implica em uma compressão maior, mas o programa executa a compressão/descompressão por mais tempo.
