module Client where

import Data.List
import System.IO
import Data.Char
import Network
import Network.Socket

--connection info
ip = "10.0.0.1"
port = 4359

--commands
fwd = "Forwards"
rwd = "Backwards"
left = "Left"
right = "Right"
stop = "Stop"

main :: IO()
main = withSocketsDo $ do
	sock <- socket AF_INET Stream defaultProtocol
	hostAddr <- inet_addr ip
	connect sock(SockAddrInet port hostAddr)
	handle <- socketToHandle sock ReadWriteMode
	hSetBuffering handle (BlockBuffering Nothing)
	doTheJob handle
	hClose handle
	sClose sock
	putStrLn "shutdown complete"
	where
		doTheJob handle = do
			putStrLn "enter the command (w/s/a/d):"
			rd <- getLine
			case rd of
				"w" -> send fwd
				"s" -> send rwd
				"a" -> send left
				"d" -> send right
				"end" -> putStrLn "shutting down"
				_ 	-> do
					putStrLn "Invalid input"
					doTheJob handle
			where
				send str = do
					hPutStr handle str
					hFlush handle
					doTheJob handle
			
					