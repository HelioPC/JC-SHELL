# Relógio lógico

O relógio lógico é uma técnica usada em sistemas distribuídos para ordenar eventos e capturar a ordem parcial entre eles, mesmo que os relógios dos diferentes nós não estejam sincronizados de maneira precisa. Ele é baseado no conceito de marcas de tempo associadas a cada evento.

Vou explicar o funcionamento detalhado do relógio lógico e fornecer um pseudocódigo para ilustrar as operações envolvidas:

1. Inicialização:
   - Cada nó mantém uma variável local chamada `clock` para representar o relógio lógico.
   - O relógio é inicializado com um valor inicial.

Pseudocódigo para inicialização:
```plaintext
clock = 0  // Valor inicial do relógio lógico
```

2. Atualização de relógio local:
   - Quando um evento ocorre em um nó, o valor do relógio lógico local é incrementado em uma unidade.

Pseudocódigo para atualização de relógio local:
```plaintext
eventoOcorreu() {
    clock = clock + 1
}
```

3. Envio de mensagens:
   - Ao enviar uma mensagem, o nó anexa o valor do seu relógio lógico à mensagem.

Pseudocódigo para envio de mensagem:
```plaintext
enviarMensagem(destino, mensagem) {
    mensagem.attach(clock)  // Anexa o valor do relógio lógico
    enviar(mensagem, destino)
}
```

4. Recebimento de mensagens:
   - Ao receber uma mensagem, o nó destinatário atualiza o seu próprio relógio lógico para o máximo entre o seu valor atual e o valor recebido na mensagem, e incrementa o relógio em uma unidade.

Pseudocódigo para recebimento de mensagem:
```plaintext
receberMensagem(origem, mensagem) {
    valorRecebido = mensagem.getAttachedClock()
    clock = max(clock, valorRecebido) + 1
    // Processar a mensagem recebida
}
```

5. Comparação de marcas de tempo:
   - A ordem parcial entre eventos pode ser estabelecida comparando as marcas de tempo associadas a eles.
   - Se um evento A possui uma marca de tempo menor que um evento B, pode-se concluir que o evento A ocorreu antes do evento B.

Pseudocódigo para comparação de marcas de tempo:
```plaintext
compararMarcasDeTempo(marca1, marca2) {
    se marca1 < marca2 {
        // O evento associado a marca1 ocorreu antes do evento associado a marca2
    } else se marca1 > marca2 {
        // O evento associado a marca2 ocorreu antes do evento associado a marca1
    } else {
        // A ordem entre os eventos é indefinida ou são eventos simultâneos
    }
}
```

O relógio lógico permite a ordenação parcial dos eventos, capturando a relação causal entre eles, independentemente dos relógios locais estarem sincronizados ou não. É importante notar que o relógio lógico não fornece uma noção precisa de tempo absoluto, mas é uma ferramenta útil para determinar a ordem parcial entre eventos em sistemas distribuídos.