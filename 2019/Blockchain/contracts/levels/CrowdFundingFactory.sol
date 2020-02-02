pragma solidity ^0.5.8;

import './base/Level.sol';
import './CrowdFunding.sol';

contract CrowdFundingFactory is Level {

    function createInstance(address _player) public payable returns (address) {
        _player;
        FundMe f = new FundMe("Securimag");
        CrowdFunding c = new CrowdFunding();
        c.register(f);
        c.fund.value(msg.value)("Securimag");
        return address(c);
    }

    function validateInstance(address _instance, address _player) public returns (bool) {
        _player;
        CrowdFunding instance = CrowdFunding(_instance);
        return instance.cleared();
    }
}
