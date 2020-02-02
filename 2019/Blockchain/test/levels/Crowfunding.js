const CrowfundingFactory = artifacts.require('CrowdFundingFactory')
const Crowfunding = artifacts.require('CrowdFunding')
const CrowfundingAttack = artifacts.require('CrowdFundingAttack')

const Ethernaut = artifacts.require('./Ethernaut.sol')

import * as utils from '../utils/TestUtils'
const truffleAssert = require('truffle-assertions');

contract('Crowfunding', function(accounts) {

    let ethernaut;
    let level;
    let owner = accounts[0];
    let player = accounts[1];


    before(async function() {
        ethernaut = await Ethernaut.new();
        level = await CrowfundingFactory.new()
        await ethernaut.registerLevel(level.address)
    });

    it('should fail if the player did not solve the level', async function() {
        const instance = await utils.createLevelInstance(ethernaut, level.address, player, Crowfunding,  {value: web3.utils.toWei("1", "ether"), from:player})
        //const instance = await utils.createLevelInstance(ethernaut, level.address, player, Crowfunding, {from: player, value: initialDeposit})

        await truffleAssert.reverts(instance.winChall())

        const completed = await utils.submitLevelInstance(
            ethernaut,
            level.address,
            instance.address,
            player
        )

        assert.isFalse(completed)
    });

    

    it('should allow the player to solve the level', async function() {
        const instance = await utils.createLevelInstance(ethernaut, level.address, player, Crowfunding, {value: web3.utils.toWei("1", "ether"), from:player})

        const balance = await web3.eth.getBalance(instance.address)
        assert.isTrue(balance == web3.utils.toWei("1", "ether"))

        const attacker = await CrowfundingAttack.new()

        await attacker.set_name('Exploit')

        await instance.register(attacker.address)
        await instance.fund('Exploit', 
            {value: web3.utils.toWei("1", "ether"), from:player}
        )
        await attacker.launch_attack(instance.address, {from:player})
        await instance.withdraw_funds('Exploit', {from:player})

        await instance.winChall()

        const completed = await utils.submitLevelInstance(
            ethernaut,
            level.address,
            instance.address,
            player
        )

        assert.isTrue(completed)
    });

});
