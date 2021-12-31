Here are some extracted and reconstructed (CRCs recalculated and VBF header added) Instrument Panel Cluster firmware files that are no longer available from the Ford download site.

AR79 are for the FG2 / SZ1

ER2T are for the FGX

FG2IPCComms.exe is an experimental Instrument Cluster Panel firmware extractor.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Updated: FG2IPCComms.exe can now write back some items to the Cluster.  *** BE WARNED *** I have bricked my cluster, requiring a full firmware reload to restore its function.  This software is by nature experimental, probably contains bugs, and should not be used on anything other than spare parts you are considering installing in your car, but which you can live without if they become damaged.  I.E. NEVER USE THIS SOFTWARE ON YOUR ORIGINAL CLUSTER TO WRITE NEW DATA, ALWAYS USE A SPARE.

For the most part (other than the splash screen updater, which needs to search for a known image before updating) you can save the hex values produced in the bottom log window into a new file (I.E. copy and paste the text exactly as it is to a file to save and then back into the bottom data Text-Box when writing) to write back original data - please make sure you always save a copy of everything before making any edits, or using any of the menu options.  ALWAYS right click the button before left clicking it (if there are options, and some buttones have them, a menu will open with what special options are set).

.CCC data can be extracted and wrtten back, with options

.EEPROM data can be extracted and written back, with options

*** This should be enough to clone your original cluster to another spare unit that won't leave you stranded if something goes wrong ***

.Splash screen images can be searched for and replaced...  ANY IMAGES SHOWN BY THIS PROGRAM ARE NOT LINKED TO THE COMPANY INVOLVED - they are simply shown so you know what you will be searching for.

.Custom images may be modified by a Gamma vScroll control - ALWAYS ENSURE YOU RECORD THE VALUE USED - when later searching for a replaced image you need to use the same Gamma setting or the splash screen image will not be found and you will need to have the original firmware restored by a professional.  Forscan Beta + extended license with firmware update may help here *** I have no relationship with the Forscan people ***

You need a real ELM327 that properly supports 125K CANBUS speeds (many clones DO NOT and will leave you thinking this software does not work) or a good quality clone, with a physical HS/MS switch (search ebay for: Forscan ELM327) and try it with Forscan first to make sure you can read the details from both the High-Speed CANBUS and Medium-Speed CANBUS in your car.  Always keep the ELM327 device on the Medium-Speed setting when using FG2IPCComms.exe since it is not designed to operate on the High-Speed CANBUS.

A final warning: Do not use this software yourself, please engage the services of trained professionals to carry out any of the things mentioned here.

I.E. If you do NOT have the ability to restore a firmware, DO NOT USE THIS SOFTWARE.  Please seek professionals to do any work using this software.
