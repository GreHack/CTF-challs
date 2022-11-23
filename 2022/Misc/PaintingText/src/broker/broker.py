#!/usr/bin/python3
# -*- coding: utf-8 -*-

from discord.ext import commands
from dotenv import load_dotenv
import discord
import os
import requests

# Defines some variables
intents = discord.Intents.default()
intents.message_content = True
bot = commands.Bot(command_prefix='!', intents=intents, help_command=None)
# load .env file
load_dotenv()

# Bot status and activity
@bot.event
async def on_ready():
    print("Ready !")
    activity = discord.Game(name="Painting beautiful texts")
    return await bot.change_presence(status=discord.Status.online, activity=activity)

# Help command function
@bot.command(name="help")
async def help_cmd(ctx):
    if ctx:
        await ctx.send("""\n**You can use theses commands :**
                        ```\n!help : print the help list\n!beautify : make your text beautiful\n exemple: !beautify me```""")
    else:
        await ctx.send("Please use !help to have list of commands")

# Ping command function
@bot.command(name="beautify")
async def beautify(ctx, parameter):
    rep = requests.post("http://chall:8000/", json={"parameter": parameter}, headers={"Content-Type": "application/json"})
    print(rep)
    final_output = rep.json().get("output", "Error, sorry.")
    await ctx.send(final_output)

bot.run(os.getenv("TOKEN"))
