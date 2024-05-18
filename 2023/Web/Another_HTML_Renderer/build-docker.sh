#!/bin/bash
docker build -t web_another_html_renderer .
docker run --name=web_another_html_renderer --rm -p5000:5000 -it web_another_html_renderer