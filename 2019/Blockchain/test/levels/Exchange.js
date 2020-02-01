const ExchangeFactory = artifacts.require('./levels/ExchangeFactory.sol')
const Exchange = artifacts.require('./levels/Exchange.sol')
const MyToken = artifacts.require('MyToken')
//const ExchangeAttack = artifacts.require('./attacks/ExchangeAttack.sol')

const Ethernaut = artifacts.require('./Ethernaut.sol')
const truffleAssert = require('truffle-assertions');

import * as utils from '../utils/TestUtils'
const { expectRevert } = require('@openzeppelin/test-helpers');

contract('Exchange', function(accounts) {

    let ethernaut;
    let level;
    let owner = accounts[0];
    let player = accounts[1];
 //   let initialDeposit = web3.utils.toWei("1", "ether")

    before(async function() {
        ethernaut = await Ethernaut.new();
        level = await ExchangeFactory.new()
        await ethernaut.registerLevel(level.address)
    });

    it('should fail if the player did not solve the level', async function() {
        const instance = await utils.createLevelInstance(ethernaut, level.address, player, Exchange)
        //const instance = await utils.createLevelInstance(ethernaut, level.address, player, Exchange, {from: owner, value: initialDeposit})

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
        //const instance = await utils.createLevelInstance(ethernaut, level.address, player, Exchange,  {from: owner, value: initialDeposit})
        const instance = await utils.createLevelInstance(ethernaut, level.address, player, Exchange)

        const token_east_addr = await instance.token_east()
        let token_east = await MyToken.at(token_east_addr)
        const token_west_addr = await instance.token_west()
        let token_west = await MyToken.at(token_west_addr)

        await token_east.increaseApproval(instance.address, 
            web3.utils.toWei("1", "ether"),
            {from:player}
        )
        await token_west.increaseApproval(instance.address, 
            web3.utils.toWei("1", "ether"), 
            {from:player}
        )

        await instance.convert_east_to_west(web3.utils.toWei("1", "ether"), {from:player})

        // just burn the tokens
        await token_west.transfer('0x0000000000000000000000000000000000000000', 
            web3.utils.toWei("1", "ether"), 
            {from:player}
        )
        await instance.convert_west_to_east(web3.utils.toWei("1", "ether"), {from:player})

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
