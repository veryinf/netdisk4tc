signin = "https://signup.live.com/signup.aspx"
description = "Microsoft SkyDrive"

local auth = function() 
    expl('https://oauth.live.com/authorize?client_id=00000000480DD833&redirect_uri=https://oauth.live.com/desktop&response_type=token&scope=wl.skydrive')
end

dir = function(params)
    auth()
--    local 
--    local files = {}
--    local i = 10
--    local qParams = {}
--    for k, v in pairs(params) do
--        i = i + 1
--        files[i] = {attribute = 'file', filename = k .. '-' .. v, size = 1, create = '2012-12-30 15:03:54', access = '2012-12-31 15:03:54', write = '2012-12-31 10:03:54'}
--    end
--    files[i + 1] = {attribute = 'file', filename = 'a.txt', size = i, create = '2012-12-30 15:03:54', access = '2012-12-31 15:03:54', write = '2012-12-31 10:03:54'}
--    qParams['url'] = 'http://imsto/gateway.php'
--    qParams['method'] = 'get'
--    qParams['body'] = 'hello=world&c3=callback'
--    local response = http(qParams)
--    local data = json.Decode(response)
--    local file = io.open("D:\\out.lua", "w")
--    file:write(response)
--    file:close()
--
--    files[i + 2] = {attribute = 'directory', filename = data.error, create = '5122445'}
--    return files
end


