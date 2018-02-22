var Client = require("ssh2").Client;
var conn = new Client();
var fs = require("fs");

// ************************************
let options = {
    host: "192.168.0.53",
    port: 22,
    username: "pi",
    password: "raspberry"
  },
  dir = "E:/www/psion-pi/keyboard/",
  target = "/home/pi/keyboard/";
// ************************************

const isDirectory = path => fs.lstatSync(path).isDirectory();

conn
  .on("ready", function() {
    conn.sftp(function(err, sftp) {
      if (err) throw err;

      // Single file upload
      //   sftp.fastPut("interface.c", "/home/pi/keyboard/dwa", function(err) {
      //     if (err) throw err;
      //     console.log("done");
      //   });

      // TO DO: Set this going as a tasty watch event
      fs.readdir(dir, function(err, names) {
        names.forEach(name => {
          let path = dir + name;
          if (!isDirectory(path)) {
            console.log("Uploading", path);
            sftp.fastPut(path, target + name, function(err) {
              if (err) throw err;
              //   console.log("Done with", path);
            });
          }
        });
      });
    });
  })
  .connect(options);
