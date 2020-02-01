pragma solidity ^0.5.8;

import './base/Level.sol';
import './Exchange.sol';

contract ExchangeFactory is Level {

    function createInstance(address _player) public payable returns (address) {
        _player;
        return address(new Exchange());
    }

    function validateInstance(address _instance, address _player) public returns (bool) {
        _player;
        Exchange instance = Exchange(_instance);
        return instance.cleared();
    }
}
