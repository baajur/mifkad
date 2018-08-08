# mifkad

[![Build Status](https://travis-ci.org/deciduously/mifkad.svg?branch=master)](https://travis-ci.org/deciduously/mifkad)

Attendance taking web app.

This is a ReasonReact/actix_web/cap'nproto rewrite of my prior [attendance](https://github.com/deciduously/attendance) Reagent/Rocket project.  As much as I love working with Clojure, I was spending more time with the tooling I felt was reasonable while porting that to re-frame and adding functionality.  Same goes for Rocket - great to work with, but nightly rust was a hassle.  It turns out actix is great, anyhow.

So, of course, I hopped ship to something else new and niche.  Let's see how this goes.

## Usage
 The build is contralled via `npm` scripts defined in `package.json`.  I prefer `yarn`, use what makes you happy.
 
 I haven't done build tooling yet, so for now, open 3 terminals (or tmux or something) and run each of these in their own:

* `yarn start` - compile Reason files to JS and watch for changes
* `yarn webpack` - Bundle the JS for serving and watch for changes
* `cargo watch -x test -x run` - Build and run the server, watching for changes and running the tests before starting back up.

Then open a browser to `localhost:8080`, pick a day, and log attendance until the cows home.

It expects an input file under `sample/current.xlsx`. There's a sample in there with fudged data.  It intentionally has mismatched headers and things - that's how they come outta the big box, except for all the extraneous info I've removed.   Redacted, if I want to sound fancy.  And I do.  Rows or data in rows that aren't what I care about don't matter.

Obviously, this is a temporary requirement - this file location will be submitted by either command line flag, env variable, or via the web frontend.
