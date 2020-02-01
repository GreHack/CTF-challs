pragma solidity ^0.5.8;

import './base/Level.sol';
import './Registers.sol';

contract RegistersFactory is Level {

    function createInstance(address _player) public payable returns (address) {
        _player;
        return address(new Registers());
    }

    function validateInstance(address _instance, address _player) public returns (bool) {
        _player;
        Registers instance = Registers(_instance);
        return instance.cleared();
    }
}
