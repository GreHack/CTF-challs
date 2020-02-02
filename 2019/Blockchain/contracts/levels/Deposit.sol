pragma solidity ^0.5.8;

contract Deposit{

    bool public cleared = false;

    mapping(address => uint) balances;

    function deposit() public payable{
        balances[msg.sender] += msg.value;
    }

    function withdrawBalance() public {
        (bool ret, bytes memory _mem) = msg.sender.call.value(balances[msg.sender])("");
        if( ! ret ){
            revert();
        }
        balances[msg.sender] = 0;
    }   

    function winChall() public{
        require(address(this).balance == 0);

        cleared = true;
    }

}