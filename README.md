# Operation: Signals & Secrets — the website

This folder is a complete, self-contained website for Auggie's summer. Every page
links to the others, with a "Mission HQ" button at the top of each one. `index.html`
is the home page.

You can open `index.html` on your own computer right now (double-click it) to try it
out. To get a real web address Auggie can visit from any device, follow the steps below.

--------------------------------------------------------------------------------
PUBLISHING TO GITHUB PAGES (free, ~10 minutes, one-time setup)
--------------------------------------------------------------------------------

1. Make a free GitHub account at https://github.com  (skip if you have one).

2. Create a new repository:
   - Click the "+" top-right → "New repository".
   - Name it something like:  signals-and-secrets
   - Set it to "Public".
   - Do NOT add a README (this folder already has one).
   - Click "Create repository".

3. Upload these files:
   - On the new repo page, click "uploading an existing file".
   - Drag in EVERY file from this folder (index.html and all the rest).
   - Click "Commit changes".

4. Turn on Pages:
   - Go to the repo's "Settings" tab → "Pages" (left sidebar).
   - Under "Branch", choose "main" and "/ (root)", then "Save".

5. Wait about 1–2 minutes, then refresh the Pages settings screen. It will show:
       Your site is live at  https://YOUR-USERNAME.github.io/signals-and-secrets/
   That link is the home page. Bookmark it on Auggie's device.

--------------------------------------------------------------------------------
GOOD TO KNOW
--------------------------------------------------------------------------------

- The .html pages open in the browser. The two special files DOWNLOAD instead of
  opening — that's on purpose:
    * mission-01-caesar-cracker.ipynb  → open it in Google Colab (colab.research.google.com)
    * mission-04-field-messenger.ino   → open it in the Arduino IDE
  The home page labels both so it's not a surprise.

- To PRINT any sheet nicely (cipher wheel, briefs, primer), open it in the browser
  and use the browser's Print command. Set scale to 100% (not "fit to page") so the
  cipher wheel stays round. The HQ nav bar is automatically hidden when printing.

- To change anything later, just upload a new version of that file to the repo —
  the website updates within a minute.

--------------------------------------------------------------------------------
WHAT'S IN HERE
--------------------------------------------------------------------------------

  index.html ........................ Mission HQ (the home page — start here)
  dossier.html ...................... The summer master plan + mind map
  briefs.html ....................... All seven mission briefs, one per page
  mission-01-kit.html ............... Mission 01 printable Codebreaker's Kit
  mission-01-caesar-cracker.ipynb ... Mission 01 Python notebook (for Colab)
  mission-02-field-pack.html ........ Mission 02 SDR + Morse + range log
  radio-primer.html ................. Illustrated primer: waves & antennas
  mission-04-field-messenger.ino .... Mission 04 Arduino code for the LoRa radios
