This level walks you through the very basics of how to play the game.

&nbsp;
#### 0. Connect to the right network

&nbsp;
#### 1. Set up MetaMask
If you don't have it already, install the [MetaMask browser extension](https://metamask.io/) (in Chrome, Firefox, Brave or Opera on your desktop machine).
You have, at your disposal, 8 different wallets filled with a bunch of Ethereum (use them wisely):

```
PLACEHOLDER_PUBLIC_KEYS
```

and their respective private keys:

```
PLACEHOLDER_PRIVATE_KEYS
```

Import at least one wallet and use the network selector to connect to our local
blockchain. 

You don't want to waste ETH (neither do we), so you better set up the right
network for these challenges to work properly: `http://TODO_IP_CHALL`

On our network, blocks are mined automatically after a few seconds, so you don't
have to wait too long to see the result of your actions.

&nbsp;
#### 2. Open the browser's console
Open your browser's console: `Tools > Developer Tools`.

You should see a few messages from the game. One of them should state your player's address. This will be important during the game! You can always see your player address by entering the following command:
```
player
```

Keep an eye out for warnings and errors, since they could provide important information during gameplay.

&nbsp;
#### 3. Use the console helpers

You can also see your current ether balance by typing:
```
await getBalance(player)
```
###### NOTE: Expand the promise to see the actual value, even if it reads "pending". If you're using Chrome v62, you can use `await getBalance(player)` for a cleaner console experience.

Great! To see what other utility functions you have in the console type:
```
help()
```
These will be super handy during gameplay.

&nbsp;
#### 4. The ethernaut contract
Enter the following command in the console:
```
ethernaut
```

This is the game's main smart contract. You don't need to interact with it directly through the console (as this app will do that for you) but you can if you want to. Playing around with this object now is a great way to learn how to interact with the other smart contracts of the game.

Go ahead and expand the ethernaut object to see what's inside.

&nbsp;
#### 5. Interact with the ABI
`ethernaut` is a `TruffleContract` object that wraps the `Ethernaut.sol` contract that has been deployed to the blockchain.

Among other things, the contract's ABI exposes all of `Ethernaut.sol`'s public methods, such as `owner`. Type the following command for example:
```
await ethernaut.owner()
```
You can see who the owner of the ethernaut contract is, which is not you of course =D.

&nbsp;
#### 6. Getting a level instance
When playing a level, you don't interact directly with the ethernaut contract. Instead, you ask it to generate a **level instance** for you. To do so, click the blue button at the bottom of the page. Go do it now and come back!

You should be prompted by MetaMask to authorize the transaction. Do so, and you should see some messages in the console.

&nbsp;
#### 7. Inspecting the contract
Just as you did with the ethernaut contract, you can inspect this contract's ABI through the console using the `contract` variable.
This is a web3 JS object representing the smart contract on the blockchain.

&nbsp;
#### 8. Validating the instance
To validate this challenge, find a way to send 10 ETH (ore more) to this level's contract and find a way to get your money
(or at least most of it) back.

#### __Tip__
__If you need (and you probably will, for the next challenges) a better way to interact with smart contract, give [Remix](https://remix.ethereum.org) a try.__

If javascript from your browser is too annoying, you can use [geth](https://github.com/ethereum/go-ethereum):
```
$geth attach http://TODO_IP_CHALL:8545 

player = web3.personal.importRawKey(PRIVATE_KEY, '')
web3.personal.unlockAccount(player)

abi = [] // solc file.sol --abi
address_contract = "0x..."
contract_instance = web3.eth.contract(abi).at(address_contract)

contract_instance.my_func(param1, param2, {from: player})

```
