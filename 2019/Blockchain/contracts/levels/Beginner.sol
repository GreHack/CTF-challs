pragma solidity ^0.5.8;

contract Beginner {

    bool public cleared = false;
    address private owner;

    constructor(address _player) public {
        owner = _player;
    }

    function sendMoneeey() public payable {
        //
    }

    function getMyMoneeeyBack() public {
        require(address(this).balance >= 10 ether, "Don't be stingy: send me moar moneeey!");
        address payable target = address(int160(owner));
        target.transfer(address(this).balance - 0.19 ether);
        cleared = true;
    }
}
