const BeginnerFactory = artifacts.require('./levels/BeginnerFactory.sol')
const Beginner = artifacts.require('./levels/Beginner.sol')
const BeginnerAttack = artifacts.require('./attacks/BeginnerAttack.sol')

const Ethernaut = artifacts.require('./Ethernaut.sol')

import * as utils from '../utils/TestUtils'
const { expectRevert } = require('@openzeppelin/test-helpers');

contract('Beginner', function(accounts) {

    let ethernaut;
    let level;
    let owner = accounts[0];
    let player = accounts[1];

    before(async function() {
        ethernaut = await Ethernaut.new();
        level = await BeginnerFactory.new()
        await ethernaut.registerLevel(level.address)
    });

    it('should fail if the player did not solve the level', async function() {
        const instance = await utils.createLevelInstance(ethernaut, level.address, player, Beginner)

        const completed = await utils.submitLevelInstance(
            ethernaut,
            level.address,
            instance.address,
            player
        )

        assert.isFalse(completed)
    });

    it('should not allow the player to solve the level with less than 10 ETH', async function() {
        const instance = await utils.createLevelInstance(ethernaut, level.address, player, Beginner)
        await instance.sendMoneeey({"value": web3.utils.toWei("9")});
        await expectRevert(instance.getMyMoneeeyBack(), "Don't be stingy: send me moar moneeey!");

        const completed = await utils.submitLevelInstance(
            ethernaut,
            level.address,
            instance.address,
            player
        )

        assert.isFalse(completed)
    });

    it('should allow the player to solve the level', async function() {
        const instance = await utils.createLevelInstance(ethernaut, level.address, player, Beginner)

        const attacker = await BeginnerAttack.new()
        await attacker.attack(instance.address, {"value": web3.utils.toWei("10")});

        const completed = await utils.submitLevelInstance(
            ethernaut,
            level.address,
            instance.address,
            player
        )

        assert.isTrue(completed)
    });

});
