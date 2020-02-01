pragma solidity ^0.5.8;

import './base/Level.sol';

contract Manufactured is Level {

    function createInstance(address _player) public payable returns (address) {
        _player;
        return address(0);
    }

    function validateInstance(address _instance, address _player) public returns (bool) {
        _player;
        _instance;
        return true;
    }

}
