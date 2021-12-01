# BTCPayServer PDV - Manual de Boas Vindas da Bitcoin Beach Brasil
Agora que você está aceitando Bitcoin como forma de pagamento, é necessário entender um pouco mais sobre o fluxo financeiro que será adotado em seu estabelecimento e maneiras de proteger seus fundos. Não se preocupe com os termos técnicos, em pouco tempo você estará familiarizado com eles e seguirá confiante em sua jornada de descoberta do Bitcoin.

A principal missão do Bitcoin é devolver o poder ao indivíduo, mas para isso é preciso se tornar um usuário soberano, mantendo a custódia do seu Bitcoin, evitando riscos desnecessários, protegendo muito bem os links de recuperação das suas carteiras e suas chaves privadas. 

Apenas para constar, na solução oferecida pelo Bitcoin Beach Brasil, utilizamos um Nó Completo do Bitcoin e Lightning Network através do MyNodeBTC. Neste servidor que roda na Vila de Jericoacoara, utilizamos o BTCPayServer, LNBits, LNDHub, entre outras soluções, todas de código aberto. 

Entre as ferramentas que utilizamos, quase todas elas estão em período de testes, mas, já tornam possível você receber Bitcoin em sua carteira (Onchain ou Lightning) de maneira fácil e prática. Pense na Lightning como sua conta de gastos diários e a sua carteira OnChain sua conta de economias para o futuro. 

Nenhuma carteira é tão segura quanto uma carteira OnChain, saiba disso. Em praticamente todos os aplicativos da Lightning existem notificações sobre os riscos e a não garantia contra falhas de segurança, porém, a Lightning possibilita que pequenos pagamentos sejam feitos em Bitcoin, o que na rede OnChain, dependendo do volume de transações, pode ser inviável, portanto, por essa razão é necessário possuir as duas carteiras. 

# SUA CARTEIRA LIGHTNING LOCAL
A carteira da Lightning que você está utilizando através da nossa solução (BlueWallet + LNBits), está hospedada em https://lnbits.jericoacoara.io:5001. Ela não é adequada para manter os fundos por longos períodos, mas sim para gastos pequenos que vão movimentar a economia local de Bitcoin da Vila, nos estabelecimentos e com os usuários que participam do experimento. 

A única informação que temos sobre sua carteira, fornecida no momento da sua adesão ao sistema, é a LND INVOICE URL, ou seja, o link para geração de faturas a serem pagas para sua carteira. Com este link, não temos acesso aos seus fundos, porém ainda sim, eles estão hospedados em nosso nó local, e existem riscos. Caso algo de errado aconteça com o servidor, ou com uma das soluções que utilizamos, os fundos podem ser perdidos.

Não queremos que isso aconteça e por isso, adotamos diversas medidas de segurança que reduzem a superfície de ataque do servidor, assim como políticas de backup para salvar informações sensíveis que não podem ser perdidas. É importante ficar claro, que nós não temos acesso a senha da sua carteira Lightning hospedada no servidor local e não podemos recuperá-la caso você perca seus links de recuperação.

# REDUÇÃO DE RISCOS
Nós não possuímos as chaves privadas da sua carteira OnChain. A única informação que é armazenada em nosso servidor é a chave pública extendida (XPUB), que fornece apenas os endereços e saldo da carteira. Proteja sua carteira no Smartphone ativando uma senha, nunca compartilhe suas chaves privadas e estará seguro. 

Já para reduzir seus riscos da sua carteira Lightning, você pode começar a utilizar suas frações de Bitcoin logo depois de recebê-las para fazer pagamentos aos seus fornecedores e funcionários, ou vendê-las para um terceiro, eliminando totalmente o seu risco. 

Dependendo da quantia, não compensa mover os fundos para uma carteira OnChain, pois as taxas podem variar de R$ 0,50 a R$ 100 dependendo do tamanho da fila de transações e da velocidade que deseja a confirmação, o que também posteriormente pode ser um problema se você precisar gastar esses fundos.

Neste caso, recomendamos que mova seus fundos para um nó da lightning mais confiável, como o nó padrão da BlueWallet https://lndhub.com que é basicamente o mesmo código que utilizamos, porém, com uma capacidade técnica melhor e maior segurança. Para isso basta criar uma nova carteira na sua Bluewallet com a configuração padrão, gerar uma fatura, e pagar para nova carteira fora do nó local. 

O cenário ideal do usuário soberano, seria manter esses fundos em seu próprio nó local e receber os pagamentos no seu próprio micro-servidor. Já para as carteiras OnChain, o ideal para armazenamento de longo prazo são as hardware wallets como a Ledger ou Trezor. 

Agora que você já sabe um pouco mais sobre o Bitcoin e a Lightning, já está preparado para ir mais fundo na toca do coelho e conhecer detalhes muito importantes da nossa operação para reduzir os riscos e prevenir falhas no sistema. 

# VALOR MÁXIMO RECOMENDADO MANTIDO NA CARTEIRA LIGHTNING LOCAL
Recomendamos que mantenha no máximo R$ 500 em sua carteira da Lightning local e mova os seus fundos para um nó mais seguro, ou carteiras OnChain o quanto antes, em no máximo 7 dias após o recebimento. 

# CAPACIDADE DE PAGAMENTO
Você pode receber pagamentos de até 600.000 SATS (0.006 BTC),  aproximadamente R$ 2.000,00 por vez, quantas vezes forem necessárias, sempre esperando 10 segundos entre uma transação e outra.

# TAXA DA TRANSAÇÃO LIGHTNING
É cobrada uma taxa de 2% por transação recebida. 
Automaticamente 1% é doado para projetos sociais locais e 1% destinado a manutenção do servidor

# TAXA DE PAGAMENTOS ONCHAIN 
Não é cobrada nenhuma taxa para transações OnChain. 


# COMO VENDER O BITCOIN
Só podemos ajudá-lo a vender bitcoin se for através de uma transação ponto-a-ponto (P2P), para um morador local. Entre em contato conosco, que lhe indicaremos um comprador em até 72h. 

# COMO MOVER BITCOINS DA CARTEIRA LIGHTNING PARA ONCHAIN 
Opção 1: Entre em contato conosco e faremos a transferência por uma taxa de 1%

Opção 2: 
Acesse CoinOS.io
Crie um usuário 
Clique em Receive, Lightning e gere uma fatura 
Após fazer o pagamento lightning, clique em Send
Insira um endereço de Bitcoin OnChain
Configure a menor tarifa possível (141 SATS), movendo a barra do fee até que chegue no valor 1000 mSATs/byte. 
Esteja ciente de que quanto menor a tarifa, mais pode demorar a confirmação

Opção 2: 
Acesse FixedFloat.com

# QUAL A FORMA MAIS SEGURA DE GERAR CARTEIRAS ONCHAIN

# QUAIS MEUS ENDEREÇOS DE BITCOIN ONCHAIN
