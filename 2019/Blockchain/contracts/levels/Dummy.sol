pragma solidity ^0.5.8;

contract Dummy {
  bool public completed;
  function setCompleted(bool _completed) public {
    completed = _completed;
  }
}
