pragma solidity ^0.5.8;

// Pseudo ERC20 tokens
// We don't respect the real specification
contract MyToken{

    mapping(address => uint) public balances;
    mapping(address => mapping(address => uint)) public allowance;

    constructor() public{
        balances[msg.sender] = 1 ether;
    }

    function transfer(address to, uint value) public returns (bool){
        if (balances[msg.sender] < value){
            return false;
        }
        balances[msg.sender] -= value;
        balances[to] += value;
        return true;
    }

    function transfer_from(address from, address to, uint value) public returns (bool){
        require(allowance[from][msg.sender] >= value);
        if (balances[from] < value){
            return false;
        }
        allowance[from][msg.sender] -= value;
        balances[from] -= value;
        balances[to] += value;
        return true;
    }

    function increaseApproval(address to, uint amount) public{
        // We dont care about overflow
        allowance[msg.sender][to] += amount;
    }

}

contract Exchange{

    bool public cleared = false;

    MyToken public token_east;
    MyToken public token_west;
    address owner;

    constructor() public{
        // Start with 10** tokens (== 1 ether)
        token_east = new MyToken();
        // Start with 10** tokens (== 1 ether)
        token_west = new MyToken();
        owner = msg.sender;
    }

    function convert_east_to_west(uint value) public{
        // 1-1 ratio
        token_east.transfer_from(msg.sender, address(this), value);
        token_west.transfer(msg.sender, value);
    } 
    
    function convert_west_to_east(uint value) public{
        // 1-1 ratio
        token_west.transfer_from(msg.sender, address(this), value);
        token_east.transfer(msg.sender, value);
    } 

    function withdraw(uint value_east, uint value_west) public{
        // This is a failsafe for tokens that are incorrectly send to this contract
        // Additionally the owner can withdraw the initial supply of the East and West tokens
        require(msg.sender == owner);

        token_east.transfer(msg.sender, value_east);
        token_west.transfer(msg.sender, value_west);
    }

    function winChall() public{
        require(token_east.balances(address(this)) == 0);
        require(token_west.balances(address(this)) == 0);

        cleared = true;
    }

}