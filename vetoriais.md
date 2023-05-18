Vou explicar o funcionamento dos relógios vetoriais com mais detalhes e fornecer pseudocódigos para ilustrar as operações envolvidas. Vamos considerar um sistema com três nós, identificados como A, B e C.

1. Inicialização:
   - Cada nó mantém um vetor de relógio com um elemento para cada nó no sistema.
   - Os vetores de relógio são inicializados com zeros em todos os elementos.

Pseudocódigo para inicialização:
```plaintext
clock[A] = [0, 0, 0]  // Vetor de relógio do nó A
clock[B] = [0, 0, 0]  // Vetor de relógio do nó B
clock[C] = [0, 0, 0]  // Vetor de relógio do nó C
```

2. Atualização de relógio local:
   - Quando um evento ocorre em um nó, seu próprio elemento no vetor de relógio é incrementado em uma unidade.

Pseudocódigo para atualização de relógio local:
```plaintext
eventoOcorreuNoNoA() {
    clock[A][A] = clock[A][A] + 1
}
```

3. Envio de mensagens:
   - Ao enviar uma mensagem, o nó anexa seu vetor de relógio atualizado à mensagem.

Pseudocódigo para envio de mensagem:
```plaintext
enviarMensagem(origem, destino, mensagem) {
    mensagem.attach(clock[origem])  // Anexa o vetor de relógio atualizado
    enviar(mensagem, destino)
}
```

4. Recebimento de mensagens:
   - Ao receber uma mensagem, o nó destinatário atualiza seu próprio vetor de relógio com os valores máximos encontrados entre seu vetor de relógio atual e o vetor recebido.

Pseudocódigo para recebimento de mensagem:
```plaintext
receberMensagem(origem, mensagem) {
    vetorRecebido = mensagem.getAttachedClock()
    para cada i de 0 até o tamanho do vetorRecebido - 1 {
        clock[destino][i] = max(clock[destino][i], vetorRecebido[i])
    }
    // Processar a mensagem recebida
}
```

5. Comparação de relógios:
   - Para comparar a ordem causal entre dois eventos, com base nos vetores de relógio, é realizada uma comparação elemento a elemento. Se todos os elementos do vetor de um nó forem menores ou iguais aos elementos correspondentes no vetor do outro nó, pode-se concluir que o primeiro evento ocorreu antes do segundo.

Pseudocódigo para comparação de relógios:
```plaintext
compararRelogios(relogio1, relogio2) {
    todosMenoresOuIguais = true
    para cada i de 0 até o tamanho do relogio1 - 1 {
        se relogio1[i] > relogio2[i] {
            todosMenoresOuIguais = false
            quebrarLoop
        }
    }
    se todosMenoresOuIguais {
        // O evento associado a relogio1 ocorreu antes do evento associado a relogio2
    } else {
        // A ordem causal entre os eventos é indefinida ou o evento associado a relogio2 ocor