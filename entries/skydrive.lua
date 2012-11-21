signin = "https://signup.live.com/signup.aspx"
description = "Microsoft SkyDrive"

local auth = function(key) 
    local url = 'https://login.live.com/oauth20_authorize.srf?client_id=00000000480DD833&scope=wl.skydrive&response_type=token&redirect_uri=http://pma.phpeye.net/callback.html?t=skydrive'
    local params = {}
    params['url'] = url
    params['width'] = 400
    params['height'] = 600
    params['title'] = "—È÷§ SkyDrive ’À∫≈"
    local token = expl(params)
    if token and token ~= '' then
        token = string.sub(token, 7)
        savt(key, token)
        return token
    end
    return nil
end

dir = function(params)
    if not params['nickname'] or params['nickname'] == '' then
        return nil
    end
    if params['token'] == '' then
        params['token'] = auth(params['nickname'])
    end
    if not params['token'] or params['token'] == '' then
        return nil
    end
    if not params['path'] or params['path'] == '' then
        params['path'] = '/'
    end
    local files = {}
    local i = 0 
    local qParams = {}
    qParams['url'] = 'https://apis.live.net/v5.0/me/skydrive/files?access_token=' .. params['token']
    qParams['method'] = 'get'
    local response = http(qParams)
    local data = json.Decode(response)
    local file = io.open("D:\\response.json", "w")
    file:write(response)
    file:close()
    local file = io.open("D:\\url.url", "w")
    file:write(qParams['url'])
    file:close()
--    for k, v in pairs(params) do
--        i = i + 1
--        files[i] = {attribute = 'file', filename = k .. '-' .. v, size = 1, create = '2012-12-30 15:03:54', access = '2012-12-31 15:03:54', write = '2012-12-31 10:03:54'}
--    end
--    files[i + 1] = {attribute = 'file', filename = 'a.txt', size = i, create = '2012-12-30 15:03:54', access = '2012-12-31 15:03:54', write = '2012-12-31 10:03:54'}
--    files[i + 2] = {attribute = 'directory', filename = string.sub(token, 0, 5), create = '5122445'}
    return files
end


