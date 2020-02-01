pragma solidity ^0.5.8;

import './base/Level.sol';
import './Beginner.sol';

contract BeginnerFactory is Level {

    function createInstance(address _player) public payable returns (address) {
        return address(new Beginner(_player));
    }

    function validateInstance(address _instance, address _player) public returns (bool) {
        _player;
        Beginner instance = Beginner(_instance);
        return instance.cleared();
    }
}
