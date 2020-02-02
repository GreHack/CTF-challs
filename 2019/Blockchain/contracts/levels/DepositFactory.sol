pragma solidity ^0.5.8;

import './base/Level.sol';
import './Deposit.sol';

contract DepositFactory is Level {

    function createInstance(address _player) public payable returns (address) {
        _player;
        Deposit d = new Deposit();
        d.deposit.value(msg.value)();
        return address(d);
    }

    function validateInstance(address _instance, address _player) public returns (bool) {
        _player;
        Deposit instance = Deposit(_instance);
        return instance.cleared();
    }
}
