pragma solidity ^0.5.8;

import './base/Level.sol';
import './SafeBank.sol';

contract SafeBankFactory is Level {

    function createInstance(address _player) public payable returns (address) {
        _player;
        SafeBank s = new SafeBank();
        s.deposit.value(msg.value)();
        return address(s);
    }

    function validateInstance(address _instance, address _player) public returns (bool) {
        _player;
        SafeBank instance = SafeBank(_instance);
        return instance.cleared();
    }
}
